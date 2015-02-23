//
//  main.cpp
//  OpenCV-FaceDetection
//
//  Created by Lakmal on 20/02/15.
//  Copyright (c) 2015 NTU. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <string>
#include <iomanip>

#include "face_detect.hpp"
#include "boost/program_options.hpp"

#define PROG_VERSION    1.0

using namespace std;

void cpuFaceDetect(string classi_val, int big_face);
void gpuFaceDetect(string classi_val, int big_face);

namespace
{
    const size_t ERROR_IN_COMMAND_LINE = 1;
    const size_t SUCCESS = 0;
    const size_t ERROR_UNHANDLED_EXCEPTION = 2;
}


int main(int argc, char** argv)
{
    try
    {
		string mode, mode_val;
		string classifier, classi_val;
        int big_face = 0;

        namespace po = boost::program_options;

        po::options_description desc("Options");
        desc.add_options()
            ("help,h", "Print help messages")
			("version,v", "Print program version")
			("mode,m", po::value<string>(&mode)->required(), "execution mode GPU or CPU")
			("classifier,c", po::value<string>(&classifier)->required(), "path to classifier xml file")
            ("show-biggest-face,b", "Show biggest face in a separate window")
        ;

		po::positional_options_description positionalOptions;
		positionalOptions.add("mode", 1);
		positionalOptions.add("classifier", 1);

		po::variables_map vm;

		try
		{
			po::store(po::command_line_parser(argc, argv).options(desc)
					  .positional(positionalOptions).run(),
					vm);

			if ( vm.count("help")  )
			{
				cout << "Face Detection implementation for embedded devices\n"
					 << " Copyright 2015 NTU" << std::endl << std::endl;
				cout << desc << endl;
				return SUCCESS;
			}

			if ( vm.count("version")  )
			{
				cout << std::fixed << std::setprecision(1)
					 << "Face Detection version " << PROG_VERSION << endl;
				return SUCCESS;
			}

			po::notify(vm);

            if ( vm.count("show-biggest-face") )
            {
                big_face = 1;
            }

		}
		catch(boost::program_options::required_option& e)
		{
			cerr << "ERROR: " << e.what() << endl << endl;
			return ERROR_IN_COMMAND_LINE;
		}
		catch(boost::program_options::error& e)
		{
			std::cerr << "ERROR: " << e.what() << endl << endl;
			return ERROR_IN_COMMAND_LINE;
		}

		mode_val = vm["mode"].as<string>();
		classi_val = vm["classifier"].as<std::string>();

		if ( mode_val == "gpu")
		{
			gpuFaceDetect(classi_val, big_face);
		}
		else if (mode_val == "cpu")
		{
			cpuFaceDetect(classi_val, big_face);
		}
		else
		{
			cout << "Unsupported mode" << endl;
		}


	}
	catch(exception& e)
	{
		cerr << "Unhandled Exception reached the top of main: "
             << e.what() << ", application will now exit" << endl;
		return ERROR_UNHANDLED_EXCEPTION;
	}

	return SUCCESS;

}
