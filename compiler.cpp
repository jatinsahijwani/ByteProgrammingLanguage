#include<string>
#include<iostream>
#include<vector>
#include<map>
#include<algorithm>
#include<fstream>
using namespace std;
vector<string> keywords;
map<int,string> intVariables;
map<char,string> characterVariables;
map<short,string> shortVariables;
map<long,string> longVariables;
map<float,string> floatVariables;
map<double,string> doubleVariables;
ofstream cppfile;
ifstream inputFile;

string checkInputFile(string jkfile)
{
	string x = " ";
	ifstream k(jkfile);
	while(1)
	{
		k>>x;
		if(k.fail()) break;

		if(binary_search(keywords.begin(),keywords.end(),x)==false)
		{
			size_t found = x.find("printnewl(");
			if(found!=string::npos && found==0)
			{
				if(*(x.end()-1)==')' && *(x.end()-2)=='"') continue;
				while(1)
				{
					k>>x;
					if(k.fail()) break;
					found=x.find(")");
					if(found==string::npos) continue;
					if(*(x.end()-1)==')' && *(x.end()-2)=='"') break;
				}
				x = " ";
				continue;
			}
			found = x.find("input");
			if(found!=string::npos && found==0) continue;
			found = x.find("print(");
			if(found!=string::npos && found==0)
			{
				if(*(x.end()-1)==')' && *(x.end()-2)=='"') continue;
				while(1)
				{
					k>>x;
					if(k.fail()) break;
					found=x.find(")");
					if(found==string::npos) continue;
					//cout<<*(x.end()-1)<<*(x.end()-2)<<endl;
					if(*(x.end()-1)==')' && *(x.end()-2)=='"') break;
				}
				x = " ";
				continue;
			}
			else return x;
		}
	}
	if(binary_search(keywords.begin(),keywords.end(),x)==false)
	{
		size_t found = x.find("print");
		if(found!=string::npos && found==0) x=" ";
		found = x.find("input");
		if(found!=string::npos && found==0) x=" ";
		found = x.find("printnewl");
		if(found!=string::npos && found==0) x=" ";
		else return x;
	}
	else x = " ";
	return x;
}
void loadKeywords()
{
	keywords.push_back("import");
	keywords.push_back("break");
	keywords.push_back("case");
	keywords.push_back("char");
	keywords.push_back("const");
	keywords.push_back("continue");
	keywords.push_back("default");
	keywords.push_back("do");
	keywords.push_back("double");
	keywords.push_back("else");
	keywords.push_back("enum");
	keywords.push_back("float");
	keywords.push_back("for");
	keywords.push_back("if");
	keywords.push_back("int");
	keywords.push_back("long");
	keywords.push_back("return");
	keywords.push_back("short");
	keywords.push_back("static");
	keywords.push_back("structure");
	keywords.push_back("switch");
	keywords.push_back("rename");
	keywords.push_back("union");
	keywords.push_back("while");
	keywords.push_back("print");
	keywords.push_back("printnewl");	
	keywords.push_back("input");
	keywords.push_back("console");
	sort(keywords.begin(),keywords.end());
}
bool checkForExtension(const char *x)
{
	const char *demo = strstr(x,".jk");
	if(demo==NULL) return false;
	else return true;
}
int main(int argc, char const *argv[])
{
	if(argc==1)
	{
		cout<<"No input files"<<endl;
		cout<<"Compilation terminated"<<endl;
		return 0;
	}
	if(checkForExtension(argv[1])==false)
	{
		cout<<"File of Unknown Type"<<endl;
		cout<<"Compilation terminated"<<endl;
		return 0;
	}
	inputFile.open(argv[1]);
	if(inputFile==NULL)
	{
		cout<<"File Not Found With Name : "<<argv[1]<<endl;
		cout<<"Compilation terminated"<<endl;
		return 0;
	}
	cppfile.open("sample.cpp");
	loadKeywords();
	string error = checkInputFile(argv[1]);
	if(error != " ")
	{
		cout<<"Unknown identifier : "<<error<<endl;
		cout<<"Compilation terminated"<<endl;
		return 0;
	}
	string focus;
	char functionName = 'a';
	while(1)
	{
		inputFile>>focus;
		if(inputFile.fail())
		{
			cppfile<<"}\nint main()\n{\na();\nreturn 0;\n}\n";
			break;
		}
		if(focus=="import") cppfile<<"#include";
		if(focus=="console") cppfile<<"<iostream>\nusing namespace std;\nvoid a()\n{\n";
		size_t found = focus.find("printnewl(");
		if(found != string::npos && found==0)
		{
			cppfile<<"cout<<";
			size_t pos1 = focus.find("(");
			size_t pos2 = focus.find(")");
			string outdump = focus.substr(pos1+1);
			if(*(outdump.end()-1)==')' && *(outdump.end()-2)=='"')
			{
				outdump.pop_back();
				cppfile<<outdump<<"<<endl;\n";	
			}
			else
			{
				cppfile<<outdump;
				inputFile>>outdump;
			while(*(outdump.end()-1)!=')' && *(outdump.end()-2)!='"')
			{
				cppfile<<" "<<outdump;
				inputFile>>outdump;
				if(inputFile.fail()) break;
			}
			outdump.pop_back();
			cppfile<<" "<<outdump<<"<<endl;\n";
			}
		}
		found = focus.find("print(");
		if(found != string::npos && found==0)
		{
		cppfile<<"cout<<";
			size_t pos1 = focus.find("(");
			size_t pos2 = focus.find(")");
			string outdump = focus.substr(pos1+1);
			if(*(outdump.end()-1)==')' && *(outdump.end()-2)=='"')
			{
				outdump.pop_back();
				cppfile<<outdump<<"<<endl;\n";	
			}
			else
			{
				cppfile<<outdump;
				inputFile>>outdump;
			while(*(outdump.end()-1)!=')' && *(outdump.end()-2)!='"')
			{
				cppfile<<" "<<outdump;
				inputFile>>outdump;
				if(inputFile.fail()) break;
			}
			outdump.pop_back();
			cppfile<<" "<<outdump<<"<<endl;\n";
		}
		}
		//testing:cout<<focus<<endl;
	}
	cppfile.close();
	//testing zone
	//system("clear");
	//system("cat sample.cpp");
	system("g++ sample.cpp -o sample.out");
	system("./sample.out");
	return 0;
}