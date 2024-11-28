#include <iostream>
#include <vector>
#include <string>

class Para {
private:	
	std::string key;
	std::string value;
public:
	Para() {}
	Para(std::string key, std::string value="");
	void set_key(std::string key);
	void set_value(std::string value);
	std::string& get_key();
	std::string& get_value();
};

Para::Para(std::string key, std::string value) {
	set_key(key);
	set_value(value);
}
void Para::set_key(std::string key) {
	this->key = key;
}
std::string& Para::get_key() {
	return this->key;
}
void Para::set_value(std::string value) {
	this->value = value;
}
std::string& Para::get_value() {
	return this->value;
}

class Form {
private:
	std::vector<Para> data;
public:
	std::string& operator[] (std::string key) {
        for (unsigned i = 0; i < data.size(); i++) {
			if (data[i].get_key() == key) {
				return data[i].get_value();
			}
		}
		data.push_back(Para(key));
		return data.back().get_value();
    }
};

std::string edit(std::string str) {
	std::string substr;
	char flag = 1;
	char simbol;
	int index;

	while(flag) {
		index = str.find("%");
		if (index != -1) {
			substr = str.substr(index+1, 2);
			str.erase(index, 3);
			simbol = stoi(substr, 0, 16);
			str.insert(index, 1, simbol);
			flag = 1;
		} else {
			flag = 0;
		}
	}
	return str;
}

Form get_form(std::string str) {
	Form form;
	std::string key;
	std::string value;
	std::string substr;
	int index;
	char flag = 1;
	
	str = edit(str);

	while (flag) {
		index = str.find("&");
		if (index != -1) {
			substr = str.substr(0, index);
			str.erase(0, index+1);
			flag = 1;
		} else {
			substr = str;
			flag = 0;
		}
		index = substr.find("=");
		key   = substr.substr(0, index);
		value = substr.substr(index+1, substr.back());
		form[key] = value;
	}
	return form;
}
