#include "IniFile.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
using namespace std;
//Эта функция удаляет пробелы из строки и обрезает строку до символа ;, если он встречается
std::string cleanupStr(std::string& str) {
	string temp = "";

	for (char c : str) {
		if (c == ';') {
			return temp;
		}
		if (c != ' ') {
			temp += c;
		}
	}
	return temp;
}

//Эта функция проверяет, является ли строка корректной: она должна быть непустой, содержать ровно одну пару квадратных скобок или не содержать их вообще.
bool clean(std::string& line) 
{
    size_t lpos = line.find('[');
    size_t rpos = line.find(']');
    if (line.empty() || lpos != string::npos && lpos != line.rfind('[') || rpos != string::npos && rpos != line.rfind(']') || lpos != string::npos && rpos == string::npos)
    {
        return false;
    }
    return true;
}

//реализация парсера
IniFile::IniFile(const std::string& path): filePath(path)
{
	ifstream file(filePath);

	if (file.is_open()) {
		string str;
		string currentSection;
		while (getline(file, str)) {
			string line = cleanupStr(str);
            size_t lpos = line.find('[');
            if (clean(line))
            {
                continue;
            }
			if (lpos != string::npos) {
				currentSection = line.substr(1, line.length() - 2);
			}
			if (currentSection.empty()) {
				continue;
			}

			size_t pos = line.find('=');
			if (pos != string::npos && (pos == line.rfind('='))) {
				string key = line.substr(0, pos);
				string value = line.substr(pos + 1);
				if (key.empty() || value.empty()) {
					continue;
				}
				data[currentSection][key] = value;
			} 
		}
		file.close();
	}
}

//it->first(секции)
//it->second(ключ.значние)
void IniFile::save()
{
    std::ofstream file;

    // Открываем файл по пути, указанному в filePath
    file.open(filePath);
    if(file.is_open())
    {
        // Итерируемся по всем секциям
        for(auto it = data.cbegin(); it != data.cend(); ++it)
        {
            // Записываем название секции в квадратных скобках
            file << "[" << it->first << "]"<< std::endl;

            // Итерируемся по всем ключам и значениям в текущей секции
            for(auto nit = it->second.cbegin(); nit != it->second.cend(); ++nit)
            {
                // Записываем ключ и значение в формате key=value
                file << nit->first << "=" << nit->second << std::endl;
            }
        }
    }
}


IniFile::~IniFile()
{
    save();
}


int IniFile::readInt(const std::string& section, const std::string& key, int def)
{
    for(auto it = data.cbegin(); it != data.cend(); ++it)
    {
         if(it ->first == section)
        {
            for(auto nit = it->second.cbegin(); nit != it->second.cend(); ++nit)
            {
                try{
                    if(nit->first == key)
                    { 
                        int ans = std::stoi(nit->second);
                        return ans;
                    }
                }
                catch (...){
                    return def;
                }
            }
            return def;
        }
        
    }
    return def;
}

double IniFile::readDouble(const std::string& section, const std::string& key, double def)
{
    for(auto it = data.cbegin(); it != data.cend(); ++it)
    {
         if(it ->first == section)
        {
            for(auto nit = it->second.cbegin(); nit != it->second.cend(); ++nit)
            {
                if(nit->first == key)
                {
                    double ans = std::stod(nit->second);
                    return ans;
                }
            }
            return def;
        }
        
    }
    return def;
}

std::string IniFile::readString(const std::string& section, const std::string& key, const std::string& def)
{
    for(auto it = data.cbegin(); it != data.cend(); ++it)
    {
         if(it ->first == section)
        {
            for(auto nit = it->second.cbegin(); nit != it->second.cend(); ++nit)
            {
                if(nit->first == key)
                {
                    return nit->second;
                }
            }
            return def;
        }
        
    }
    return def;
}


bool IniFile::readBool(const std::string& section, const std::string& key, bool def)
{
    for(auto it = data.cbegin(); it != data.cend(); ++it)
    {
         if(it ->first == section)
        {
            for(auto nit = it->second.cbegin(); nit != it->second.cend(); ++nit)
            {
                if(nit->first == key)
                {
                    for(int i = 0; i < trueValues.size(); ++i)
                    {
                        if(trueValues[i] == nit ->second)
                        {
                            return true;
                        }
                    }
                    return false;
                }
            }
            return def;
        }
        
    }
    return def;
}


void IniFile::writeInt(const std::string& section, const std::string& key, int value)
{
    data[section][key] = std::to_string(value);
}

void IniFile::writeDouble(const std::string& section, const std::string& key, double value)
{
    data[section][key] = std::to_string(value);
}
void IniFile::writeString(const std::string& section, const std::string& key, const std::string& value)
{
    data[section][key] = value;
}
void IniFile::writeBool(const std::string& section, const std::string& key, bool value)
{
    data[section][key] = std::to_string(value);
}

 bool IniFile::isSectionExist(const std::string& section)
 {
    for(auto it = data.cbegin(); it != data.cend(); ++it)
    {
        if(it->first == section)
            return true;
    }
    return false;
 }

 bool IniFile::isKeysExist(const std::string& section, const std::string& key)
 {
    for(auto it = data[section].cbegin(); it != data[section].cend(); ++it)
    {
        if(it->first == key)
            return true;
    }
    return false;
 }

 size_t IniFile::getSectionsCount()
 {
    size_t count = 0;
    for(auto it = data.cbegin(); it != data.cend(); ++it)
    {
        if(!it->second.empty())
        count++;
    }
    return count;
 }

 size_t IniFile::getKeysCount(const std::string& section)
 {
    size_t count = 0;
    
    for(auto it = data[section].cbegin(); it != data[section].cend(); ++it)
    {
        count++;
    }
    return count;
 }

 SectionsMap IniFile::getSections() const
 {
    return data;
 }

bool IniFile::deleteSection(const std::string& section)
{
    return data.erase(section);
}

bool IniFile::deleteKey(const std::string& section,const std::string& key)
{
    bool ans = false;
    for(auto it = data[section].cbegin(); it != data[section].cend(); ++it)
    {
        if(key == it->first)
            ans = true;
    }
    if(ans)
    {
        data[section].erase(key);
    }
    return ans;
}

bool IniFile::addNewSection(const std::string& section)
{
    if(data.count(section) == 0)
    {
        data[section] = KeysMap();
        return true;
    }
    return false;
}