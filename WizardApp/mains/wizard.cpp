#include <iostream>
#include <thread>
#include <chrono>
#include <string>

#include "../src/include/BuildManager.hpp"
#include "../src/include/logger.hpp"

int main(int argc, char **argv)
{
	std::string gamePath;
	std::string outputPath;
	std::string configFile;
	bool SAMPObjs = true;
	bool CustomObjs = true;

	if (argc < 3 || argc > 4)
	{
		std::cout << "Usage: " << argv[0] << " <game path> <output path> <config_file> ";
		return 0;
	}

	open_log_file("log.txt");

	gamePath = argv[1];
	outputPath = argv[2];
	configFile = argv[3] ? argv[3] : "default.cfg";

	BuildManager *thisBuild = new BuildManager(gamePath, configFile, outputPath, SAMPObjs, CustomObjs);

	log_message("Processing: Image files (.img)");
	thisBuild->ExtractImageFiles();
	log_message("Done.");

	log_message("Processing: Collision files (.col)");
	thisBuild->ExtractCollisionFiles();
	log_message("Done.");

	log_message("Processing: Item placement files (.ipl)");
	thisBuild->ExtractItemPlacementFiles();
	log_message("Done.");

	log_message("Processing: Item definition files (.ide)");
	thisBuild->ExtractItemDefinitionFiles();
	log_message("Done.");

	log_message("Preparing database structures, this may take some time.");
	thisBuild->PrepareDatabaseStructures();
	log_message("Done.");

	log_message("Writing the database file.");
	thisBuild->WriteBinaryFile(outputPath.c_str());
	log_message("Done.");

	log_end();

	return 0;
}
