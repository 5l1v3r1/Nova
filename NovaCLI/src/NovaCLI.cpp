//============================================================================
// Name        : NovaCLI.cpp
// Copyright   : DataSoft Corporation 2011-2012
//	Nova is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
//   Nova is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with Nova.  If not, see <http://www.gnu.org/licenses/>.
// Description : Command line interface for Nova
//============================================================================

#include "NovaCLI.h"
#include "nova_ui_core.h"
#include "Logger.h"


#include <iostream>
#include <stdlib.h>
#include "inttypes.h"
#include "boost/program_options.hpp"


namespace po = boost::program_options;
using namespace std;
using namespace Nova;
using namespace NovaCLI;


int main(int argc, const char *argv[])
{
	// Fail if no arguements
	if(argc < 2)
	{
		PrintUsage();
	}

	Config::Inst();

	// Disable notifications and email in the CLI
	Logger::Inst()->SetUserLogPreferences(LIBNOTIFY, EMERGENCY, '+');
	Logger::Inst()->SetUserLogPreferences(EMAIL, EMERGENCY, '+');
	InitializeUI();

	// We parse the input arguments here,
	// but refer to other functions to do any
	// actual work.

	// Listing suspect IP addresses
	if(!strcmp(argv[1], "list"))
	{
		if(argc < 3)
		{
			PrintUsage();
		}

		if(!strcmp(argv[2], "all"))
		{
			PrintSuspectList(SUSPECTLIST_ALL);
		}
		else if(!strcmp(argv[2], "hostile"))
		{
			PrintSuspectList(SUSPECTLIST_HOSTILE);
		}
		else if(!strcmp(argv[2], "benign"))
		{
			PrintSuspectList(SUSPECTLIST_BENIGN);
		}
		else
		{
			PrintUsage();
		}
	}

	// Checking status of components
	else if(!strcmp(argv[1], "status"))
	{
		if(argc < 3)
		{
			PrintUsage();
		}

		if(!strcmp(argv[2], "nova"))
		{
			StatusNovaWrapper();
		}
		else if(!strcmp(argv[2], "haystack"))
		{
			StatusHaystackWrapper();
		}
		else
		{
			PrintUsage();
		}
	}

	// Starting components
	else if(!strcmp(argv[1], "start"))
	{
		if(argc < 3)
		{
			PrintUsage();
		}

		if(!strcmp(argv[2], "nova"))
		{
			StartNovaWrapper();
		}
		else if(!strcmp(argv[2], "haystack"))
		{
			if (argc > 3)
			{
				if (!strcmp(argv[3], "debug"))
				{
					StartHaystackWrapper(true);
				}
				else
				{
					PrintUsage();
				}
			}
			else
			{
				StartHaystackWrapper(false);
			}

		}
		else if (!strcmp(argv[2], "capture"))
		{
			StartCaptureWrapper();
		}
		else
		{
			PrintUsage();
		}
	}

	// Stopping components
	else if(!strcmp(argv[1], "stop"))
	{
		if(argc < 3)
		{
			PrintUsage();
		}

		if(!strcmp(argv[2], "nova"))
		{
			StopNovaWrapper();
		}
		else if(!strcmp(argv[2], "haystack"))
		{
			StopHaystackWrapper();
		}
		else if (!strcmp(argv[2], "capture"))
		{
			StopCaptureWrapper();
		}
		else
		{
			PrintUsage();
		}
	}

	// Getting suspect information
	else if(!strcmp(argv[1], "get"))
	{
		if(argc < 3)
		{
			PrintUsage();
		}

		if(!strcmp(argv[2], "all"))
		{
			if(argc == 4 && !strcmp(argv[3], "csv"))
			{
				PrintAllSuspects(SUSPECTLIST_ALL, true);
			}
			else
			{
				PrintAllSuspects(SUSPECTLIST_ALL, false);
			}
		}
		else if(!strcmp(argv[2], "hostile"))
		{
			if(argc == 4 && !strcmp(argv[3], "csv"))
			{
				PrintAllSuspects(SUSPECTLIST_HOSTILE, true);
			}
			else
			{
				PrintAllSuspects(SUSPECTLIST_HOSTILE, false);
			}
		}
		else if(!strcmp(argv[2], "benign"))
		{
			if(argc == 4 && !strcmp(argv[3], "csv"))
			{
				PrintAllSuspects(SUSPECTLIST_BENIGN, true);
			}
			else
			{
				PrintAllSuspects(SUSPECTLIST_BENIGN, false);
			}
		}
		else if (!strcmp(argv[2], "data"))
		{
			if (argc != 4)
			{
				PrintUsage();
			}

			in_addr_t address;
			if(inet_pton(AF_INET, argv[3], &address) != 1)
			{
				cout << "Error: Unable to convert to IP address" << endl;
				exit(EXIT_FAILURE);
			}

			PrintSuspectData(address);

		}
		else
		{
			// Some early error checking for the
			in_addr_t address;
			if(inet_pton(AF_INET, argv[2], &address) != 1)
			{
				cout << "Error: Unable to convert to IP address" << endl;
				exit(EXIT_FAILURE);
			}

			PrintSuspect(address);
		}
	}

	// Clearing suspect information
	else if(!strcmp(argv[1], "clear"))
	{
		if(argc < 3)
		{
			PrintUsage();
		}

		if(!strcmp(argv[2], "all"))
		{
			ClearAllSuspectsWrapper();
		}
		else
		{
			ClearSuspectWrapper(argv[2]);
		}
	}

	// Checking status of components
	else if(!strcmp(argv[1], "uptime"))
	{
		PrintUptime();
	}


	else if(!strcmp(argv[1], "readsetting"))
	{
		if(argc < 3)
		{
			PrintUsage();
		}

		cout << Config::Inst()->ReadSetting(string(argv[2])) << endl;
	}

	else if(!strcmp(argv[1], "writesetting"))
	{
		if(argc < 4)
		{
			PrintUsage();
		}

		cout << Config::Inst()->WriteSetting(string(argv[2]), string(argv[3])) << endl;
	}

	else if(!strcmp(argv[1], "listsettings"))
	{
		vector<string> settings = Config::Inst()->GetPrefixes();
		for (uint i = 0; i < settings.size(); i++)
		{
			cout << settings[i] << endl;
		}
	}

	else
	{
		PrintUsage();
	}

	return EXIT_SUCCESS;
}

namespace NovaCLI
{

void PrintUsage()
{
	cout << "Usage:" << endl;
	cout << "    " << EXECUTABLE_NAME << " status nova|haystack" << endl;
	cout << "    " << EXECUTABLE_NAME << " start nova|capture|haystack [debug]" << endl;
	cout << "    " << EXECUTABLE_NAME << " stop nova|capture|haystack" << endl;
	cout << "    " << EXECUTABLE_NAME << " list all|hostile|benign" << endl;
	cout << "    " << EXECUTABLE_NAME << " get all|hostile|benign [csv]" << endl;
	cout << "    " << EXECUTABLE_NAME << " get xxx.xxx.xxx.xxx" << endl;
	cout << "    " << EXECUTABLE_NAME << " clear all" << endl;
	cout << "    " << EXECUTABLE_NAME << " clear xxx.xxx.xxx.xxx" << endl;
	cout << "    " << EXECUTABLE_NAME << " writesetting SETTING VALUE" << endl;
	cout << "    " << EXECUTABLE_NAME << " readsetting SETTING" << endl;
	cout << "    " << EXECUTABLE_NAME << " listsettings" << endl;
	cout << endl;

	exit(EXIT_FAILURE);
}

void StatusNovaWrapper()
{
	if(!ConnectToNovad())
	{
		cout << "Novad Status: Not running" << endl;
	}
	else
	{
		if(IsNovadUp(false))
		{
			cout << "Novad Status: Running and responding" << endl;
		}
		else
		{
			cout << "Novad Status: Not responding" << endl;
		}

		CloseNovadConnection();
	}
}

void StatusHaystackWrapper()
{
	if(IsHaystackUp())
	{
		cout << "Haystack Status: Running" << endl;
	}
	else
	{
		cout << "Haystack Status: Not running" << endl;
	}
}

void StartNovaWrapper()
{
	if(!ConnectToNovad())
	{
		if(StartNovad())
		{
			cout << "Started Novad" << endl;
		}
		else
		{
			cout << "Failed to start Novad" << endl;
		}
	}
	else
	{
		cout << "Novad is already running" << endl;
		CloseNovadConnection();
	}
}

void StartHaystackWrapper(bool debug)
{
	if(!IsHaystackUp())
	{
		if(StartHaystack(debug))
		{
			cout << "Started Haystack" << endl;
		}
		else
		{
			cout << "Failed to start Haystack" << endl;
		}
	}
	else
	{
		cout << "Haystack is already running" << endl;
	}
}

void StopNovaWrapper()
{
	Connect();

	if(StopNovad())
	{
		cout << "Novad has been stopped" << endl;
	}
	else
	{
		cout << "There was a problem stopping Novad" << endl;
	}
}

void StopHaystackWrapper()
{
	if(StopHaystack())
	{
		cout << "Haystack has been stopped" << endl;
	}
	else
	{
		cout << "There was a problem stopping the Haystack" << endl;
	}
}

void StartCaptureWrapper()
{
	Connect();
	StartPacketCapture();
}

void StopCaptureWrapper()
{
	Connect();
	StopPacketCapture();
}

void PrintSuspect(in_addr_t address)
{
	Connect();

	Suspect *suspect = GetSuspect(ntohl(address));

	if(suspect != NULL)
	{
		cout << suspect->ToString() << endl;
	}
	else
	{
		cout << "Error: No suspect received" << endl;
	}

	delete suspect;

	CloseNovadConnection();
}

void PrintSuspectData(in_addr_t address)
{
	Connect();

	Suspect *suspect = GetSuspectWithData(ntohl(address));

	if (suspect != NULL)
	{
		cout << suspect->ToString() << endl;


		cout << "Details follow" << endl;
		cout << suspect->GetFeatureSet(MAIN_FEATURES).toString() << endl;
	}
	else
	{
		cout << "Error: No suspect received" << endl;
	}

	delete suspect;

	CloseNovadConnection();


}

void PrintAllSuspects(enum SuspectListType listType, bool csv)
{
	Connect();

	vector<in_addr_t> *suspects;
	suspects = GetSuspectList(listType);

	if(suspects == NULL)
	{
		cout << "Failed to get suspect list" << endl;
		exit(EXIT_FAILURE);
	}

	for(uint i = 0; i < suspects->size(); i++)
	{
		Suspect *suspect = GetSuspect(suspects->at(i));

		if(suspect != NULL)
		{
			if(!csv)
			{
				cout << suspect->ToString() << endl;
			}
			else
			{
				cout << suspect->GetIpString() << ",";
				for(int i = 0; i < DIM; i++)
				{
					cout << suspect->GetFeatureSet().m_features[i] << ",";
				}
				cout << suspect->GetClassification() << endl;
			}

			delete suspect;
		}
		else
		{
			cout << "Error: No suspect received" << endl;
		}
	}

	CloseNovadConnection();

}

void PrintSuspectList(enum SuspectListType listType)
{
	Connect();

	vector<in_addr_t> *suspects;
	suspects = GetSuspectList(listType);


	if(suspects == NULL)
	{
		cout << "Failed to get suspect list" << endl;
		exit(EXIT_FAILURE);
	}

	for(uint i = 0; i < suspects->size(); i++)
	{
		in_addr tmp;
		tmp.s_addr = htonl(suspects->at(i));
		char *address = inet_ntoa((tmp));
		cout << address << endl;
	}

	CloseNovadConnection();
}

void ClearAllSuspectsWrapper()
{
	Connect();

	if(ClearAllSuspects())
	{
		cout << "Suspects have been cleared" << endl;
	}
	else
	{
		cout << "There was an error when clearing the suspects" << endl;
	}

	CloseNovadConnection();
}

void ClearSuspectWrapper(string address)
{
	Connect();

	if(ClearSuspect(address))
	{
		cout << "Suspect data has been cleared for this suspect" << endl;
	}
	else
	{
		cout << "There was an error when trying to clear the suspect data for this suspect" << endl;
	}

	CloseNovadConnection();
}

void PrintUptime()
{
	Connect();
	cout << "Uptime is: " << GetStartTime() << endl;
	CloseNovadConnection();
}

void Connect()
{
	if(!ConnectToNovad())
	{
		cout << "ERROR: Unable to connect to Nova" << endl;
		exit(EXIT_FAILURE);
	}
}

}
