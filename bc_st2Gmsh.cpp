#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{

    std::string fname;
    if(argc>1)
    {
        fname = argv[1];
    }
    else //terminal
    {
        cout << "\nInput file name:\n ";
        cin >> fname;
    }

    ifstream is(fname.data());
    if (!is.good())
    {
        cout << "File "<< fname << "can not be opened.\n";

        return EXIT_FAILURE;
    }

    while (!is.eof())
    {
        string s_buff;
        getline(is, s_buff);
        if (  s_buff.find("Neumann") != string::npos
                ||s_buff.find("Dirchilet") != string::npos )
        {
            string ofname = fname + "_source_term_nodes";
            if (s_buff.find("Dirchilet") != string::npos )
                ofname = fname + "_Dirchilet_BC_nodes";

            getline(is, s_buff);
            stringstream ss;
            ss.str(s_buff);
            int nn;
            ss >> s_buff >> s_buff >> s_buff >> nn;
            ss.clear();

            getline(is, s_buff);

            vector<string> var_names;
            vector<std::size_t> name_id(nn);
            vector<std::size_t> id(nn);
            vector<double> x(nn);
            vector<double> y(nn);
            vector<double> z(nn);
            for (int i=0; i<nn; i++)
            {
                double T;
                is >> id[i] >> s_buff
                   >> x[i] >> y[i] >> z[i] >> T >> ws;

                std::size_t nm_id = 0;
                bool existed = false;
                for (size_t j=0; j < var_names.size(); j++)
                {
                    if (s_buff.compare(var_names[j]) == 0)
                    {
                        existed = true;
                        name_id[i] = j;
                        break;
                    }
                }
                if (!existed)
                {
                    name_id[i] = var_names.size();
                    var_names.push_back(s_buff);
                }
            }

            vector<ofstream*> os(var_names.size());

            for(std::size_t i=0; i<var_names.size(); i++)
            {
                string ofname_var = ofname + "_" +var_names[i] + ".geo";
                os[i] = new ofstream(ofname_var.data(), ios::trunc);
                *os[i] << "ds=1.;\n";
            }

            for (int i=0; i<nn; i++)
            {
                *os[name_id[i]] << "Point(" << id[i] << ") = {"
                                <<  x[i] << ","
                                <<  y[i] << ","
                                <<  z[i] << ", ds};\n";
            }

            for(std::size_t i=0; i<os.size(); i++)
            {
                os[i]->close();
                delete os[i];
                os[i] = NULL;
            }

        }
    }
    is.close();

    return EXIT_SUCCESS;
}
