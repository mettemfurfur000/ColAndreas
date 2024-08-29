/*
 * This file is part of ColAndreas.
 * Copyright(C) <2013>  <christopher michael james>
 * computer_customs@hotmail.com
 *
 * ColAndreas is free software : you can redistribute it and / or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ColAndreas is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Special thanks:
 *
 * [uL]Slice, Writing the original collision reader function
 */

#include <sstream>
#include <cstring>

#include "include/BuildManager.hpp"
#include "include/ImgManager.hpp"
#include "include/ColManager.hpp"
#include "include/logger.hpp"

#include "../dirent/include/dirent.h"

#define SEPARATOR '\\'

char *get_filename(char *path)
{
	char *filename = strrchr(path, SEPARATOR);
	if (filename == NULL)
		return path;
	return filename + 1;
}

int find_all_files(const char *path, std::map<const std::string, const std::string> &filename_path_map)
{
	DIR *directory;
	struct dirent *entry;

	directory = opendir(path);

	if (directory == NULL)
	{
		printf("Unable to open directory: %s\n", path);
		closedir(directory);
		return -1;
	}

	char file_to_parse[256] = {};

	while ((entry = readdir(directory)) != NULL)
	{
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
			continue;

		snprintf(file_to_parse, sizeof(file_to_parse), "%s%c%s", path, SEPARATOR, entry->d_name);

		if (entry->d_type == DT_REG)
			filename_path_map.insert(std::make_pair(std::string(entry->d_name), std::string(file_to_parse)));

		if (entry->d_type == DT_DIR)
			find_all_files(file_to_parse, filename_path_map);
	}

	closedir(directory);

	return 0;
}

// BuildManager constructor
BuildManager::BuildManager(std::string &GTADirectory, std::string &ConfigFile, std::string &OutDirectory, bool SAMPObjects, bool CustomObjects)
	: GTAInstallDirectory(GTADirectory), OutfileDirectory(OutDirectory), UsingSAMPObjects(SAMPObjects), UsingCustomObjects(CustomObjects)
{
	InitDirectoryNames(ConfigFile);
}

FILE *BuildManager::GenerateDefaultConfig(const char *out)
{
	FILE *f = fopen(out, "w");

	if (!f)
	{
		log_message("Error opening file %s\n", out);
		return 0;
	}

	fputs("#ColAndreas Configuration File\n"
		  "#This file is used to configure the build process.\n"
		  "#\n",
		  f);

	fputs("img\n", f); //----------[ IMG ]----------//

	fputs("gta_int.img\n"
		  "gta3.img\n"
		  "SAMPCOL.img\n"
		  "custom.img\n",
		  f);

	fputs("col\n", f); //----------[ COL ]----------//

	fputs("weapons.col\n"
		  "vehicles.col\n",
		  f);
	// CollisionFileNames.push_back("\\models\\coll\\peds.col\n"; //Unusual file structure causing problems, file is unused in game.

	fputs("ipl\n", f); //----------[ IPL ]----------//

	fputs("countn2.ipl\n"
		  "countrye.ipl\n"
		  "countryN.ipl\n"
		  "countryS.ipl\n"
		  "countryw.ipl\n"
		  "gen_int1.ipl\n"
		  "int_cont.ipl\n"
		  "int_veg.ipl\n"
		  "stadint.ipl\n"
		  "LAe.ipl\n"
		  "LAe2.ipl\n"
		  "LAhills.ipl\n"
		  "LAn.ipl\n"
		  "LAn2.ipl\n"
		  "LAs.ipl\n"
		  "LAs2.ipl\n"
		  "LAw.ipl\n"
		  "LAw2.ipl\n"
		  "LaWn.ipl\n"
		  "levelmap.ipl\n"
		  "seabed.ipl\n"
		  "SFe.ipl\n"
		  "SFn.ipl\n"
		  "SFs.ipl\n"
		  "SFSe.ipl\n"
		  "SFw.ipl\n"
		  "vegasE.ipl\n"
		  "vegasN.ipl\n"
		  "vegasS.ipl\n"
		  "vegasW.ipl\n"
		  "vegaxref.ipl\n"
		  "SAMP.ipl\n",
		  f);

	fputs("ide\n", f); //----------[ IDE ]----------//

	fputs("countn2.ide\n"
		  "countrye.ide\n"
		  "countryN.ide\n"
		  "countryS.ide\n"
		  "countryW.ide\n"
		  "counxref.ide\n"
		  "barriers.ide\n"
		  "dynamic.ide\n"
		  "dynamic2.ide\n"
		  "multiobj.ide\n"
		  "procobj.ide\n"
		  "vegepart.ide\n"
		  "gen_int1.ide\n"
		  "gen_int2.ide\n"
		  "gen_int3.ide\n"
		  "gen_int4.ide\n"
		  "gen_int5.ide\n"
		  "gen_intb.ide\n"
		  "int_cont.ide\n"
		  "int_LA.ide\n"
		  "int_SF.ide\n"
		  "int_veg.ide\n"
		  "propext.ide\n"
		  "props.ide\n"
		  "props2.ide\n"
		  "savehous.ide\n"
		  "stadint.ide\n"
		  "LAe.ide\n"
		  "LAe2.ide\n"
		  "LAhills.ide\n"
		  "LAn.ide\n"
		  "LAn2.ide\n"
		  "LAs.ide\n"
		  "LAs2.ide\n"
		  "LAw.ide\n"
		  "LAw2.ide\n"
		  "LaWn.ide\n"
		  "LAxref.ide\n"
		  "leveldes.ide\n"
		  "levelmap.ide\n"
		  "levelxre.ide\n"
		  "seabed.ide\n"
		  "SFe.ide\n"
		  "SFn.ide\n"
		  "SFs.ide\n"
		  "SFSe.ide\n"
		  "SFw.ide\n"
		  "SFxref.ide\n"
		  "vegasE.ide\n"
		  "VegasN.ide\n"
		  "VegasS.ide\n"
		  "VegasW.ide\n"
		  "vegaxref.ide\n"
		  "veh_mods.ide\n"
		  "SAMP.ide\n"
		  "CUSTOM.ide\n",
		  f);

	fputs("end\n", f);
	fclose(f);

	f = fopen(out, "r");

	return f;
}

void BuildManager::ReadConfig(const char *config_file)
{
	enum READMODES
	{
		MODE_NONE,
		MODE_IMG,
		MODE_COL,
		MODE_IPL,
		MODE_IDE,
		MODE_END
	};

	FILE *f = fopen(config_file, "r");
	if (!f)
	{
		log_message("Failed to open config file: %s\n", config_file);
		const char def_config[] = "default.cfg";
		log_message("Generated default config file: %s\n", def_config);
		f = GenerateDefaultConfig(def_config);
		if (!f)
			exit(1);
	}

	std::map<const std::string, const std::string> file_map;
	find_all_files(GTAInstallDirectory.c_str(), file_map);

	char line[256];
	int mode = MODE_NONE;

	log_message("Reading config file: %s\n", config_file);

	while (fgets(line, sizeof(line), f))
	{
		if (line[0] == '#' || line[0] == '\n' || line[0] == '\r' || line[0] == '\0')
			continue;

		line[strlen(line) - 1] = '\0';
		// printf("%s\n", line);

		if (strcmp(line, "img") == 0)
		{
			mode = MODE_IMG;
			continue;
		}
		else if (strcmp(line, "col") == 0)
		{
			mode = MODE_COL;
			continue;
		}
		else if (strcmp(line, "ipl") == 0)
		{
			mode = MODE_IPL;
			continue;
		}
		else if (strcmp(line, "ide") == 0)
		{
			mode = MODE_IDE;
			continue;
		}
		else if (strcmp(line, "end") == 0)
		{
			mode = MODE_END;
			break;
		}

		std::string filename = line;
		std::string file_path = file_map[filename];

		// printf("[%s]\n", file_path.c_str());

		switch (mode)
		{
		case MODE_IMG:
			ImageFileNames.push_back(file_path);
			break;
		case MODE_COL:
			CollisionFileNames.push_back(file_path);
			break;
		case MODE_IPL:
			ItemPlacementFileNames.push_back(file_path);
			break;
		case MODE_IDE:
			ItemDefinitionFileNames.push_back(file_path);
			break;
		default:
			break;
		}

		if (feof(f))
			break;
	}

	fclose(f);
}

void BuildManager::InitDirectoryNames(std::string config_file)
{
	ReadConfig(config_file.c_str());
}

bool BuildManager::ReadColFile(const char fname[])
{
	std::ifstream file(fname, std::ios::binary);

	if (!file.is_open())
	{ // Will error on 2 files because of the path's
		// cout << "(Failed to open) Error on file: " << fname << '';
		return false;
	}

	ColFile fileReader(&file);
	ColModel modelData;

	while (fileReader.Load(&modelData))
	{
		std::transform(modelData.name.begin(), modelData.name.end(), modelData.name.begin(), ::tolower);
		if (modelData.name.find("lod") == std::string::npos)
			COLArray.emplace_back(modelData);
	}

	return true;
}

bool BuildManager::ExtractImageFiles()
{
	for (std::string &name : ImageFileNames)
	{
		log_message("\tOpening archive: %s ", name.c_str());

		ImgFile thisFile(name);

		if (thisFile.Load())
		{
			for (ImgEntry &e : thisFile.COL)
			{
				log_message("\t\tLoading: %s", e.name);

				thisFile.handle->seekg(e.offset * 2048);

				ColFile fileReader(thisFile.handle);
				ColModel modelInfo;

				while (fileReader.Load(&modelInfo))
				{
					std::transform(modelInfo.name.begin(), modelInfo.name.end(), modelInfo.name.begin(), ::tolower);
					if (modelInfo.name.find("lod") == std::string::npos)
						COLArray.emplace_back(modelInfo);
				}
			}

			for (ImgEntry &e : thisFile.IPL)
			{
				log_message("\t\tLoading: %s", e.name);

				char *data = new char[e.streamingSize * 2048];

				thisFile.handle->seekg(e.offset * 2048);
				thisFile.handle->read(data, e.streamingSize * 2048);

				ReadBinaryItemPlacementData(data);

				delete[] data;
			}

			log_message("\t\tDone!");
		}
		else
		{
			log_message("\t\tERROR: Failed to open file!");
		}
	}

	return true;
}

// throws: invalid allocation size: 4294967295 error. I assume this is because these files are of version "COLL" / version 1
bool BuildManager::ExtractCollisionFiles()
{
	bool ReturnValue = true;
	for (std::string &col : CollisionFileNames)
	{
		log_message("\tLoading: %s", col.c_str());

		if (ReadColFile((col).c_str()))
		{
			log_message("\t\tDone!");
		}
		else
		{
			log_message("\t\tERROR: Failed to open file!");
			ReturnValue = false;
		}
	}
	return true;
}

void BuildManager::ReadBinaryItemPlacementData(char *RawIPLData)
{
	uint32_t NumberofItemInstances, FileOffset;

	memcpy(&NumberofItemInstances, RawIPLData + 4, 4);
	memcpy(&FileOffset, RawIPLData + 28, 4);

	uint32_t TempModelid;

	for (uint32_t i = 0; i < NumberofItemInstances; i++)
	{
		IPLObject thisObject;

		GetBytes(RawIPLData, thisObject.Position.x, FileOffset, 4);
		GetBytes(RawIPLData, thisObject.Position.y, FileOffset, 4);
		GetBytes(RawIPLData, thisObject.Position.z, FileOffset, 4);

		GetBytes(RawIPLData, thisObject.Rotation.x, FileOffset, 4);
		GetBytes(RawIPLData, thisObject.Rotation.y, FileOffset, 4);
		GetBytes(RawIPLData, thisObject.Rotation.z, FileOffset, 4);
		GetBytes(RawIPLData, thisObject.Rotation.w, FileOffset, 4);

		GetBytes(RawIPLData, TempModelid, FileOffset, 4);
		thisObject.modelid = static_cast<uint16_t>(TempModelid);

		std::transform(thisObject.Modelname.begin(), thisObject.Modelname.end(), thisObject.Modelname.begin(), ::tolower);
		IPLArray.emplace_back(thisObject);

		FileOffset += 8; // Skip Interiorid, lodflag
	}
}

void purify_string(char *s)
{
	char *d = s;
	do
	{
		// while (*d == ' ' || *d == '\r' || *d == '\n')
		while (isspace(*d))
		{
			++d;
		}
	} while (*s++ = *d++);
}

bool BuildManager::ReadItemPlacementData(const char fname[])
{
	std::ifstream IPLFile(fname, std::ios::binary);

	if (IPLFile.is_open())
	{
		char IPLHeader[5];
		IPLHeader[4] = 0x00;

		IPLFile.read(IPLHeader, 4);
		IPLFile.seekg(0);

		if (strcmp(IPLHeader, "bnry") != 0)
		{
			std::string thisLine;
			bool inInstArea = false;

			while (std::getline(IPLFile, thisLine))
			{
				if (thisLine.empty())
					continue;
				if (thisLine.at(0) == '#')
					continue;

				purify_string(thisLine.data());

				// if (thisLine == "inst")
				if (strcmp(thisLine.c_str(), "inst") == 0)
				{
					inInstArea = true;
				}
				// else if (thisLine == "end")
				else if (strcmp(thisLine.c_str(), "end") == 0)
				{
					inInstArea = false;
				}
				else if (inInstArea)
				{
					std::stringstream paramStream(thisLine);
					std::string thisParam;
					unsigned int index = 0;
					IPLObject thisObject;

					while (std::getline(paramStream, thisParam, ','))
					{
						if (thisLine.empty())
							continue;

						if (thisLine.at(0) == '#')
							continue;

						purify_string(thisParam.data());
						// thisParam.erase(thisParam.begin(), std::find_if(thisParam.begin(), thisParam.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
						// thisParam.erase(std::find_if(thisParam.rbegin(), thisParam.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), thisParam.end());

						index++;

						try
						{
							switch (index)
							{
							case 1:
							{
								thisObject.modelid = static_cast<uint16_t>(std::stoi(thisParam));
								break;
							}
							case 2:
							{
								thisObject.Modelname = thisParam;
								break;
							}
							case 4:
							{
								thisObject.Position.x = std::stof(thisParam);
								break;
							}
							case 5:
							{
								thisObject.Position.y = std::stof(thisParam);
								break;
							}
							case 6:
							{
								thisObject.Position.z = std::stof(thisParam);
								break;
							}
							case 7:
							{
								thisObject.Rotation.x = std::stof(thisParam);
								break;
							}
							case 8:
							{
								thisObject.Rotation.y = std::stof(thisParam);
								break;
							}
							case 9:
							{
								thisObject.Rotation.z = std::stof(thisParam);
								break;
							}
							case 10:
							{
								thisObject.Rotation.w = std::stof(thisParam);
								break;
							}
							}
						}
						catch (std::invalid_argument e)
						{
							return false;
						}
						catch (std::out_of_range e)
						{
							return false;
						}
					}

					std::transform(thisObject.Modelname.begin(), thisObject.Modelname.end(), thisObject.Modelname.begin(), ::tolower);
					IPLArray.emplace_back(thisObject);
				}
			}

			return true;
		}
	}

	return false;
}

bool BuildManager::ExtractItemPlacementFiles()
{
	bool ReturnValue = true;

	for (std::string &ipl : ItemPlacementFileNames)
	{
		if (ReadItemPlacementData((ipl).c_str()))
		{
			log_message("\tLoaded: %s", ipl.c_str());
		}
		else
		{
			log_message("\tFailed to load: %s", ipl.c_str());
			ReturnValue = false;
		}
	}

	return true;
}

bool BuildManager::ExtractItemDefinitionFiles()
{
	bool ReturnValue = true;

	for (std::string &ide : ItemDefinitionFileNames)
	{
		if (ReadItemDefinitionFile((ide).c_str()))
		{
			log_message("\tLoaded: %s", ide.c_str());
		}
		else
		{
			log_message("\tFailed to load: %s", ide.c_str());
			ReturnValue = false;
		}
	}

	return ReturnValue;
}

bool BuildManager::ReadItemDefinitionFile(const char fname[])
{
	std::ifstream IDEFile(fname, std::ios::binary);

	if (IDEFile.is_open())
	{
		std::string thisLine;
		unsigned int inArea = 0;

		while (std::getline(IDEFile, thisLine))
		{
			if (thisLine.empty())
				continue;

			if (thisLine.at(0) == '#')
				continue;

			purify_string(thisLine.data());
			// thisLine.erase(std::find_if(thisLine.rbegin(), thisLine.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), thisLine.end());

			// if (thisLine == "end")
			if (strcmp(thisLine.data(), "end") == 0)
			{
				inArea = 0;
			}
			// else if (thisLine == "objs" || thisLine == "tobj")
			else if (strcmp(thisLine.data(), "objs") == 0 || strcmp(thisLine.data(), "tobj") == 0)
			{
				inArea = 1;
			}
			// else if (thisLine == "anim")
			else if (strcmp(thisLine.data(), "anim") == 0)
			{
				inArea = 2;
			}
			// else if (thisLine == "weap")
			else if (strcmp(thisLine.data(), "weap") == 0)
			{
				inArea = 3;
			}
			// else if (inArea != 0)
			else if (inArea != 0)
			{
				std::stringstream paramStream(thisLine);
				std::string thisParam;
				unsigned int index = 0;
				Modelid_Modelname_Struct thisObject;

				while (std::getline(paramStream, thisParam, ','))
				{
					if (thisLine.empty())
						continue;

					if (thisLine.at(0) == '#')
						continue;

					purify_string(thisParam.data());
					// thisParam.erase(thisParam.begin(), std::find_if(thisParam.begin(), thisParam.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
					// thisParam.erase(std::find_if(thisParam.rbegin(), thisParam.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), thisParam.end());

					index++;

					try
					{
						switch (index)
						{
						case 1:
						{
							thisObject.modelid = static_cast<uint16_t>(std::stoi(thisParam));
							break;
						}
						case 2:
						{
							thisObject.ModelName = thisParam;
							std::transform(thisObject.ModelName.begin(), thisObject.ModelName.end(), thisObject.ModelName.begin(), ::tolower);
							break;
						}
						default:
						{
							if (index == (inArea + 3))
								thisObject.DrawDist = std::stof(thisParam);
							break;
						}
						}
					}
					catch (std::invalid_argument e)
					{
						log_message("\tERROR: std::invalid_argument on %d", thisObject.modelid);
						return false;
					}
					catch (std::out_of_range e)
					{
						log_message("\tERROR: std::out_of_range on %d", thisObject.modelid);
						return false;
					}
				}

				if (!thisObject.ModelName.length())
					log_message("\tERROR: Object %d has empty name.", thisObject.modelid);

				modelnameLookup.insert(thisObject.ModelName, thisObject.modelid);
				modelidLookup.emplace(std::pair<int, std::string>(thisObject.modelid, thisObject.ModelName));

				Modelid_Modelname_Vector.emplace_back(thisObject);
			}
		}

		return true;
	}

	return false;
}

void map_printer(std::map<std::string, int> &m)
{
	std::vector<std::string> key;
	std::vector<int> value;

	for (std::map<std::string, int>::iterator it = m.begin(); it != m.end(); ++it)
	{
		key.push_back(it->first);
		value.push_back(it->second);
		std::cout << it->first << " " << it->second << std::endl;
	}
}

void BuildManager::PrepareDatabaseStructures()
{
	bool ModelidMatch, ModelnameMatch, EntryFound;

	for (auto it = COLArray.begin(); it != COLArray.end();)
	{
		auto model_it = modelidLookup.find(it->modelid);

		if (model_it == modelidLookup.end() || model_it->second != it->name)
		{
			ATree<int>::iterator name_it;

			if (modelnameLookup.find(it->name, name_it))
			{
				it->modelid = name_it->second;
			}
			else
			{
				log_message("\tWARN: Collision %s is unused, ignoring.", it->name.c_str());
				it = COLArray.erase(it);
				continue;
			}
		}

		++it;
	}

	// Alphasorting COL array
	std::sort(COLArray.begin(), COLArray.end(), [](ColModel const &a, ColModel const &b)
			  { return a.modelid < b.modelid; });

	for (auto it = IPLArray.begin(); it != IPLArray.end();)
	{
		auto coll_it = std::find_if(COLArray.begin(), COLArray.end(), [&it](ColModel &m)
									{ return it->modelid == m.modelid; });
		if (coll_it == COLArray.end())
		{
			// No collision
			it = IPLArray.erase(it);
			continue;
		}

		++it;
	}
}

bool BuildManager::WriteBinaryFile(const char fname[])
{
	// Begin writing file
	std::ofstream ColAndreasBinaryfile(fname, std::ios::trunc | std::ios::binary);

	if (ColAndreasBinaryfile.is_open())
	{
		// Filetype in header (standard binary file format) "Otherwise you could give it any old file and it would load it."
		char FileExtension[5] = "cadf";
		ColAndreasBinaryfile.write((char *)&FileExtension, 4);

		// Version
		uint16_t dbVersion = CA_DATABASE_VERSION;
		ColAndreasBinaryfile.write((char *)&dbVersion, 2);

		// Number of collision files
		uint16_t colCount = COLArray.size();
		ColAndreasBinaryfile.write((char *)&colCount, sizeof(colCount));
		log_message("\tModels: %u", colCount);

		// Number of item placment entries
		uint32_t iplCount = IPLArray.size();
		ColAndreasBinaryfile.write((char *)&iplCount, sizeof(iplCount));
		log_message("\tItem entries: %u", iplCount);

		log_message("\tAll models and item entries:");

		for (ColModel &m : COLArray)
		{
			// Modelid
			ColAndreasBinaryfile.write((char *)&(m.modelid), sizeof(m.modelid));

			// number of collision spheres
			uint16_t sphereCount = static_cast<uint16_t>(m.num_spheres);
			ColAndreasBinaryfile.write((char *)&sphereCount, sizeof(sphereCount));

			// number of collision boxes
			uint16_t boxCount = static_cast<uint16_t>(m.num_boxes);
			ColAndreasBinaryfile.write((char *)&boxCount, sizeof(boxCount));

			// number of collision faces
			uint16_t faceCount = static_cast<uint16_t>(m.num_faces);
			ColAndreasBinaryfile.write((char *)&faceCount, sizeof(faceCount));

			log_message("%s %d %d %d %d", m.name.c_str(), m.modelid, sphereCount, boxCount, faceCount);

			// Sphere data
			for (int i = 0; i < m.num_spheres; ++i)
			{
				ColAndreasBinaryfile.write((char *)(m.spheres + i), sizeof(ColSphere));
			}

			// Box data
			for (int i = 0; i < m.num_boxes; ++i)
			{
				// Convert the data to save CPU ticks, when the ColAndreas plugin loads
				float CenterX = (m.boxes[i].min.x + m.boxes[i].max.x) / 2.0f;
				float CenterY = (m.boxes[i].min.y + m.boxes[i].max.y) / 2.0f;
				float CenterZ = (m.boxes[i].min.z + m.boxes[i].max.z) / 2.0f;

				float BoxsizeX = (m.boxes[i].max.x - m.boxes[i].min.x) / 2.0f;
				float BoxsizeY = (m.boxes[i].max.y - m.boxes[i].min.y) / 2.0f;
				float BoxsizeZ = (m.boxes[i].max.z - m.boxes[i].min.z) / 2.0f;

				ColAndreasBinaryfile.write((char *)&CenterX, 4);
				ColAndreasBinaryfile.write((char *)&CenterY, 4);
				ColAndreasBinaryfile.write((char *)&CenterZ, 4);

				ColAndreasBinaryfile.write((char *)&BoxsizeX, 4);
				ColAndreasBinaryfile.write((char *)&BoxsizeY, 4);
				ColAndreasBinaryfile.write((char *)&BoxsizeZ, 4);
			}

			// Face data
			for (int i = 0; i < m.num_faces; ++i)
			{
				ColAndreasBinaryfile.write((char *)m.faces[i].a, sizeof(Vertex));
				ColAndreasBinaryfile.write((char *)m.faces[i].b, sizeof(Vertex));
				ColAndreasBinaryfile.write((char *)m.faces[i].c, sizeof(Vertex));
			}
		}

		// IPL modelid, x,y,z x,y,z,w
		for (IPLObject &o : IPLArray)
		{
			o.Rotation.w *= -1.0f;

			ColAndreasBinaryfile.write((char *)&(o.modelid), sizeof(uint16_t));
			ColAndreasBinaryfile.write((char *)&(o.Position), sizeof(Vertex));
			ColAndreasBinaryfile.write((char *)&(o.Rotation), sizeof(IPLRot));
		}

		log_message("\tFile size: %.2f MiB", float(ColAndreasBinaryfile.tellp()) / (1024.0f * 1024.0f));

		return true;
	}
	return false;
}
