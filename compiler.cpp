#include<sstream>
#include<string>
#include<iostream>
#include<vector>
#include<map>
#include<algorithm>
#include<fstream>
using namespace std;
vector<string> keywords;
map<string,char> characterVariables;
map<string,long> longVariables;
map<string,double> doubleVariables;
map<string,string> stringVariables;
ofstream cppfile;
ifstream inputFile;
bool lookForKeywords(string &x)
{
	return binary_search(keywords.begin(),keywords.end(),x);
}
size_t FindUnaryOperator(string &x)
{
	size_t found;
	found = x.find("++");
	if(found!=string::npos) return found;
	found = x.find("--");
	if(found!=string::npos) return found;
	return string::npos;
}
size_t FindDualOperator(string &x)
{
	size_t found;
	found = x.find("+=");
	if(found!=string::npos) return found;
	found = x.find("-=");
	if(found!=string::npos) return found;
	found = x.find("*=");
	if(found!=string::npos) return found;
	found = x.find("/=");
	if(found!=string::npos) return found;
	return string::npos;
}
size_t FindOperator(string &x)
{
	size_t found;
	found = x.find("+");
	if(found!=string::npos) return found;
	found = x.find("-");
	if(found!=string::npos) return found;
	found = x.find("*");
	if(found!=string::npos) return found;
	found = x.find("/");
	if(found!=string::npos) return found;
	found = x.find("%");
	if(found!=string::npos) return found;
	return string::npos;
}
void assignVariables(string &variableName,long x)
{
	longVariables[variableName]=x;
	cppfile<<variableName<<'='<<x<<';'<<endl;
}
void assignVariables(string &variableName,double x)
{
	doubleVariables[variableName]=x;
	cppfile<<variableName<<'='<<x<<';'<<endl;
}
void assignVariables(string &variableName,string x)
{
	stringVariables[variableName]=x;
	cppfile<<variableName<<'='<<x<<';'<<endl;
}
void assignVariables(string &variableName,char x)
{
	characterVariables[variableName]=x;
	cppfile<<variableName<<'='<<x<<';'<<endl;
}
string checkDatatype(string &name)
{
	string x;
	map<string,char>::iterator it1;
	map<string,long>::iterator it2;
	map<string,double>::iterator it3;
	map<string,string>::iterator it4;
	it1 = characterVariables.find(name);
	if(it1 != characterVariables.end()) return "char";
	it2 = longVariables.find(name);
	if(it2 != longVariables.end()) return "long";
	it3 = doubleVariables.find(name);
	if(it3 != doubleVariables.end()) return "double";
	it4 = stringVariables.find(name);
	if(it4 != stringVariables.end()) return "string";
	return "error";
}
void initializeVariables(string &variableName,long x)
{
	longVariables[variableName]=x;
	cppfile<<"long "<<variableName<<'='<<x<<';'<<endl;
}
void initializeVariables(string &variableName,char x)
{
	characterVariables[variableName]=x;
	cppfile<<"char "<<variableName<<'='<<"'"<<x<<"'"<<';'<<endl;
}
void initializeVariables(string &variableName,double x)
{
	doubleVariables[variableName]=x;
	cppfile<<"double "<<variableName<<'='<<x<<';'<<endl;
}
void initializeVariables(string &variableName,string x)
{

	stringVariables[variableName]=x;
	cppfile<<"string "<<variableName<<'='<<x<<';'<<endl;
}
bool createVariable(string &variableName,string &variableValue)
{
	if(*(variableValue.begin())==')' && *(variableValue.end()-1)==')')
	{
		cppfile<<variableName<<'='<<variableValue<<endl;
		return true;
	}
	if(*(variableValue.begin())==39)
	{
		if(variableValue.size()>3) return false;
		if(*(variableValue.end()-1)!=39) return false;
		else
		{
			string type = checkDatatype(variableName);
			if(type=="error")
			{
				initializeVariables(variableName,*(variableValue.end()-2));
				return true;	
			}
			else if(type=="char")
			{
				assignVariables(variableName,*(variableValue.end()-2));
				return true;
			}
			else return false;
		}
	}
	if(*(variableValue.begin())=='"')
	{
		while(*(variableValue.end()-1)!='"')
		{
			string space = " ";
			string x;
			inputFile>>x;
			if(inputFile.fail()) break;
			variableValue=variableValue+space+x;	
		}
		if(*(variableValue.end()-1)=='"')
		{
			string type = checkDatatype(variableName);
			if(type=="error")
			{
				initializeVariables(variableName,variableValue);
				return true;	
			}
			else if(type=="stirng")
			{
				assignVariables(variableName,*(variableValue.end()-2));
				return true;
			}
			else return false;
		}
	}
	string::iterator i;
	size_t found;
	int count=0;
	for(i=variableValue.begin();i!=variableValue.end();i++) if(*i=='.') count++;
	if(count==1)
	{
		size_t opt = FindOperator(variableValue);
		if (opt==string::npos)
		{
			stringstream value(variableValue);
			double x;
			value>>x;
			string type = checkDatatype(variableName);
			if(type=="error")
			{
				initializeVariables(variableName,x);
				return true;	
			}
			else if(type=="double")
			{
				assignVariables(variableName,x);
				return true;
			}
			else return false;
		}
		else
		{
			string type = checkDatatype(variableName);
			stringstream x(variableValue);
			double y=0.0,z;
			char opt;
			while(1)
			{
				x>>z;
				if(x.fail()) break;
				x>>opt;
				if(opt=='+') y+=z;
				if(opt=='-') y-=z;
				if(opt=='*') y*=z;
				if(opt=='/') y/=z;
			}
			doubleVariables[variableName]=y;
			if(type=="error")
			{
				cppfile<<"double "<<variableName<<'='<<variableValue<<';'<<endl;
				return true;
			}
			if(type=="double")
			{
				cppfile<<variableName<<'='<<variableValue<<';'<<endl;
				return true;
			}
			return false;
		}
	}
	if(count==0)
	{
		size_t opt = FindOperator(variableValue);
		if (opt==string::npos)
		{
			stringstream value(variableValue);
			long x;
			value>>x;
			string type = checkDatatype(variableName);
			if(type=="error")
			{
				initializeVariables(variableName,x);
				return true;	
			}
			else if(type=="long")
			{
				assignVariables(variableName,x);
				return true;
			}
			else return false;
		}
		else
		{
			size_t opt = FindOperator(variableValue);
			string datatype;
			if(opt!=string::npos)
			{
				string dump = variableValue.substr(0,opt);
				datatype = checkDatatype(dump);
			}
			else datatype = "error";
			if(datatype=="long")
			{
			string type = checkDatatype(variableName);
			stringstream x(variableValue);
			long y=0,z;
			char opt;
			while(1)
			{
				x>>z;
				if(x.fail()) break;
				x>>opt;
				if(opt=='+') y+=z;
				if(opt=='-') y-=z;
				if(opt=='*') y*=z;
				if(opt=='/') y/=z;
				if(opt=='%') y%=z;
			}
			longVariables[variableName]=y;
			if(type=="error")
			{
				cppfile<<"long "<<variableName<<'='<<variableValue<<';'<<endl;
				return true;
			}
			if(type=="long")
			{
				cppfile<<variableName<<'='<<variableValue<<';'<<endl;
				return true;
			}
			return false;	
			}
			if(datatype=="double")
			{
			string type = checkDatatype(variableName);
			stringstream x(variableValue);
			double y=0,z;
			char opt;
			while(1)
			{
				x>>z;
				if(x.fail()) break;
				x>>opt;
				if(opt=='+') y+=z;
				if(opt=='-') y-=z;
				if(opt=='*') y*=z;
				if(opt=='/') y/=z;
			}
			doubleVariables[variableName]=y;
			if(type=="error")
			{
				cppfile<<"double "<<variableName<<'='<<variableValue<<';'<<endl;
				return true;
			}
			if(type=="double")
			{
				cppfile<<variableName<<'='<<variableValue<<';'<<endl;
				return true;
			}
			return false;		
			}
			return false;
		}
	}
	else return false;
}
string checkInputFile(string jkfile)
{
	string x = " ";
	ifstream k(jkfile);
	while(1)
	{
		k>>x;
		if(k.fail()) break;
		for(string::iterator i = x.begin();i!=x.end();i++) if(*i==';') return ";";
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
					if(*(x.end()-1)==')') break;
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
					if(*(x.end()-1)==')' && *(x.end()-2)=='"') break;
				}
				x = " ";
				continue;
			}
			found = x.find("=");
			if(found!=string::npos)
			{
				if(found==0 || *(x.end()-1)=='=')
				{
					cout<<"Illegal use of = operator"<<endl;
					cout<<"Compilation terminated"<<endl;
					return "=";
				}
				else
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
	keywords.push_back("endelse");
	keywords.push_back("start");
	keywords.push_back("endif");
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
	const char *demo = strstr(x,".byte");
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
			cppfile.seekp(0,ios::end);
			int length = cppfile.tellp();
			if(length==0) cppfile<<"int main()\n{\n";
			cppfile<<"\nreturn 0;\n}\n";
			break;
		}
		if(focus=="import")
		{
			cppfile<<"#include";
			inputFile>>focus;
			if(focus=="console")
			{
				cppfile<<"<iostream>\nusing namespace std;\n";
			}
			//else if()
			else
			{
				cout<<"Illegal use of import"<<endl;
				cout<<"Interpretation terminated"<<endl;
				return 0;
			}
			cppfile<<"int main()\n{\n";
		}
		size_t found = focus.find("printnewl(");
		if(found != string::npos && found==0)
		{
			cppfile<<"cout<<";
			size_t pos1 = focus.find("(");
			size_t pos2 = focus.find(")");
			if(pos2==pos1+1) 
			{
				cppfile<<"endl;\n";
				continue;
			}
			string outdump = focus.substr(pos1+1);
			if(*(outdump.end()-1)==')' && *(outdump.end()-2)=='"')
			{
				outdump.pop_back();
				cppfile<<outdump<<"<<endl;\n";	
			}
			else if(*(outdump.end()-1)==')' && *(outdump.end()-2)!='"')
			{
				outdump.pop_back();
				size_t opt = FindOperator(outdump);
				string outdump2;
				if(opt!=string::npos)
				{
					outdump2=outdump.substr(0,opt);
				}
				else outdump2=outdump;
				string type = checkDatatype(outdump2);
				if(type=="error")
				{
					cout<<"Unknown Variable Used in printnewl()"<<endl;
					cout<<"Interpretation terminated"<<endl;
					system("rm sample.cpp");
					return 0;
				}
				cppfile<<outdump<<"<<endl;\n";
			}
			else
			{
			cppfile<<outdump;
			inputFile>>outdump;
			if(outdump=="+")
			{
				cppfile<<"<<";
				inputFile>>outdump;
			}
			while(*(outdump.end()-1)!=')')
			{
				cppfile<<" "<<outdump;
				inputFile>>outdump;
				if(inputFile.fail()) break;
				if(outdump=="+")
				{
					cppfile<<"<<";
					inputFile>>outdump;
				}
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
				cppfile<<outdump<<";\n";	
			}
			else if(*(outdump.end()-1)==')' && *(outdump.end()-2)!='"')
			{
				outdump.pop_back();
				string type = checkDatatype(outdump);
				if(type=="error")
				{
					cout<<"Unknown Variable Used in printnewl()"<<endl;
					cout<<"Interpretation terminated"<<endl;
					system("rm sample.cpp");
					return 0;
				}
				cppfile<<outdump<<";\n";
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
			cppfile<<" "<<outdump<<";\n";
			}	
		}
		found = focus.find("=");
		if(found!=string::npos)
		{
			if(found==0 || *(focus.end()-1)=='=')
			{
				cout<<"Illegal use of = operator"<<endl;
				cout<<"Compilation terminated"<<endl;
				return 0;
			}
			string variableName = focus.substr(0,found);
			string variableValue = focus.substr(found+1);
			bool kcheck = lookForKeywords(variableName);
			if(kcheck==true)
			{
				cout<<"Cannot use keyword as Variable"<<endl;
				cout<<"Interpretation terminated"<<endl;
				return 0;
			}
			string type = checkDatatype(variableValue);
			size_t opt = FindOperator(variableValue);
			if(opt!=string::npos)
			{
				string operand1 = variableValue.substr(opt+1);
				string operand2 = variableValue.substr(0,opt);
			}
			bool success = createVariable(variableName,variableValue);
			if(success == false)
			{
				cout<<"Illegal use of variables"<<endl;
				cout<<"Compilation terminated"<<endl;
				return 0;
			}	
		}
		found = focus.find("input(");
		if(found!=string::npos && found==0)
		{
			cppfile<<"cin>>";
			size_t pos1 = focus.find("(");
			size_t pos2 = focus.find(")");
	--			string outdump = focus.substr(pos1+1);
			if(*(outdump.end()-1)==')')
			{
				outdump.pop_back();
				string type = checkDatatype(outdump);
				if(type=="error")
				{
					cout<<"Variable Not found, used in input()"<<endl;
					cout<<outdump<<endl;
					cout<<"Interpretation terminated"<<endl;
					system("rm sample.cpp");
					return 0;
				}
				cppfile<<outdump<<";\n";	
			}			
		}
		if(focus=="if")
		{
			cppfile<<endl<<"if(";
			inputFile>>focus;
			while(focus!="start")
			{
				cppfile<<focus<<' ';	
				inputFile>>focus;
			}
		}
		if(focus=="endif")
		{
			cppfile<<"}\n";
		}
		if(focus=="while")
		{
			cppfile<<endl<<"while(";
			inputFile>>focus;
			while(focus!="start")
			{
				cppfile<<focus<<' ';	
				inputFile>>focus;
				if(inputFile.fail()) break;
			}
		}
		if(focus=="endwhile")
		{
			cppfile<<"}\n";
		}
		if(focus=="else") 
		{
			cppfile<<"else\n{";
		}
		if(focus=="start") cppfile<<')'<<'\n'<<'{'<<'\n';
		if(focus=="endelse")
		{
			cppfile<<"}\n";
		}
	}
	cppfile.close();
	//testing zone
//	system("clear");
//	system("cat sample.cpp");
	system("g++ sample.cpp -o sample.out");
	system("./sample.out");
	system("rm sample.cpp && rm sample.out");
	return 0;
}