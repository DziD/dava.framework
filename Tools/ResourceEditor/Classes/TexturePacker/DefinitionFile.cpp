/*
 *  DefinitionFile.cpp
 *  texturepack
 *
 *  Created by Vitaliy Borodovsky on 10/28/08.
 *  Copyright 2008 DAVA Consulting, LLC. All rights reserved.
 *
 */

#include "DefinitionFile.h"
#include "CommandLineParser.h"
#include "PngImage.h"
#include <sys/types.h>
#include <sys/stat.h>

DefinitionFile::DefinitionFile()
:	frameCount(0)
,	spriteWidth(0)
,	spriteHeight(0)
,	frameRects(0)
{
}

DefinitionFile::~DefinitionFile()
{
	SafeDeleteArray(frameRects);
}

bool DefinitionFile::LoadPNGDef(const std::string & _filename, const std::string & pathToProcess)
{
	if (CommandLineParser::Instance()->GetVerbose())printf("* Load PNG Definition: %s\n", _filename.c_str()); 
	
	FILE * fp = fopen(_filename.c_str(), "rt");
	fscanf(fp, "%d", &frameCount);

	String path;
	String name;
	CommandLineParser::SplitFilePath(_filename, path, name);
	String nameWithoutExt = name.substr(0, name.length() - 7);
	String corespondingPngImage = path + String("/") + nameWithoutExt + String(".png");

	filename = pathToProcess + String("/") + nameWithoutExt + String(".txt");
	
	PngImageExt image;
	image.Read(corespondingPngImage.c_str());
	spriteWidth = image.GetWidth() / frameCount;
	spriteHeight = image.GetHeight();
	
//	String dirWrite = path + String("/$process/"); 
//	mkdir(dirWrite.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (CommandLineParser::Instance()->GetVerbose())printf("* frameCount: %d spriteWidth: %d spriteHeight: %d\n", frameCount, spriteWidth, spriteHeight); 


	frameRects = new Rect2i[frameCount];
	for (int k = 0; k < frameCount; ++k)
	{
		PngImageExt frameX;
		frameX.Create(spriteWidth, spriteHeight);
		frameX.DrawImage(0, 0, &image, Rect2i(k * spriteWidth, 0, spriteWidth, spriteHeight));
		
		
		Rect2i reducedRect;
		frameX.FindNonOpaqueRect(reducedRect);
		if (CommandLineParser::Instance()->GetVerbose())printf("%s - reduced_rect(%d %d %d %d)\n", nameWithoutExt.c_str(), reducedRect.x, reducedRect.y, reducedRect.dx, reducedRect.dy);
		
		/*if (k == 1)
		{
			for (int y = 0; y < spriteWidth; ++y)
			{
				for (int x = 0; x < spriteWidth; ++x)
				{
					printf("%02x ", frameX.GetPixel(x, y)[3]);
				}
				printf("\n");
			}
		}*/
		
		PngImageExt frameX2;
		frameX2.Create(reducedRect.dx, reducedRect.dy);
		frameX2.DrawImage(0, 0, &frameX, reducedRect);
		
		char number[10];
		sprintf(number, "%d", k);
		String fileWrite = pathToProcess + String("/") + nameWithoutExt + String(number) + String(".png"); 
		frameX2.Write(fileWrite.c_str());		
	
		frameRects[k].x = reducedRect.x;
		frameRects[k].y = reducedRect.y;
		frameRects[k].dx = reducedRect.dx;
		frameRects[k].dy = reducedRect.dy;
	
	
		if (CommandLineParser::Instance()->IsFlagSet("--add0pixel"))
		{
			
		}else if (CommandLineParser::Instance()->IsFlagSet("--add1pixel"))
		{
			frameRects[k].dx++;
			frameRects[k].dy++;
		}
		else if (CommandLineParser::Instance()->IsFlagSet("--add2pixel"))
		{
			frameRects[k].dx+=2;
			frameRects[k].dy+=2;
		}
		else if (CommandLineParser::Instance()->IsFlagSet("--add4pixel"))
		{
			frameRects[k].dx+=4;
			frameRects[k].dy+=4;
		}else 
		{
			frameRects[k].dx++;
			frameRects[k].dy++;	
		}
	}
	

	fclose(fp);
	return true;
}

bool DefinitionFile::Load(const std::string & _filename)
{
	filename = _filename;
	FILE * fp = fopen(filename.c_str(), "rt");
	if (!fp)
	{
		printf("*** ERROR: Can't open definition file: %s\n",filename.c_str()); 
		return false;
	}
	fscanf(fp, "%d %d", &spriteWidth, &spriteHeight);
	fscanf(fp, "%d", &frameCount);
	
	frameRects = new Rect2i[frameCount];
	
	for (int i = 0; i < frameCount; ++i)
	{
		fscanf(fp, "%d %d %d %d\n", &frameRects[i].x, &frameRects[i].y, &frameRects[i].dx, &frameRects[i].dy);
		printf("[DefinitionFile] frame: %d w: %d h: %d\n", i, frameRects[i].dx, frameRects[i].dy);
		
		if (CommandLineParser::Instance()->IsFlagSet("--add0pixel"))
		{
			
		}else if (CommandLineParser::Instance()->IsFlagSet("--add1pixel"))
		{
			frameRects[i].dx++;
			frameRects[i].dy++;
		}
		else if (CommandLineParser::Instance()->IsFlagSet("--add2pixel"))
		{
			frameRects[i].dx+=2;
			frameRects[i].dy+=2;
		}
		else if (CommandLineParser::Instance()->IsFlagSet("--add4pixel"))
		{
			frameRects[i].dx+=4;
			frameRects[i].dy+=4;
		}else 
		{
			frameRects[i].dx++;
			frameRects[i].dy++;	
		}
	}
	
	while(1)
	{
		char tmpString[512];
		fgets(tmpString, sizeof(tmpString), fp);
		pathsInfo.push_back(tmpString);
		printf("str: %s\n", tmpString);
		if (feof(fp))break;
	}
	
	
	fclose(fp);
	printf("Loaded definition: %s frames: %d\n",filename.c_str(), frameCount); 
	
	return true;
}

