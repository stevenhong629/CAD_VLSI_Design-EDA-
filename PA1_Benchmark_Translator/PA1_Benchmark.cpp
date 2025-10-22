
//

#include<iostream>
#include<fstream>
#include<iomanip>
#include<string>
#include<cstdlib>
#include<vector>
#include<istream>
#include<sstream>
using namespace std;

struct box {
	string address;
	string name;
	string type;
	int fanout = -1;
	int fanin = -1;
	string gate_from;   //來自fanout有值時, 這個就會出來
	vector <string> fanout_number;  //當fanout幾條 就會開幾個vector的值出來
};

int main(int argc, char* argv[]) {
	vector<struct box>databox;
	// input .isc file 
	ifstream inc17(argv[1], ios::in);

	// output .v file
	stringstream ss(argv[2]);
	string module_name;
	getline(ss, module_name, '.');
	ofstream outc17(argv[2], ios::out);

	// check if files were input and output
	if (!inc17 || !outc17) {
		if (!inc17 && outc17) {
			cerr << "inc17 could not be opened." << endl;
			exit(1);
		}
		if (inc17 && !outc17) {
			cerr << "outFile could not be opened." << endl;
			exit(1);
		}
		if (!inc17 && !outc17) {
			cerr << "inc17 and outFile could not be opened." << endl;
			exit(1);
		}
		else cerr << "undefined error!";
	}

	char examination;
	int i = 0;
	int fanout_tw;
	int fanin_tw;
	string s, address_tw, type_tw, gate_from_tw, name_tw, temporary;
	// s代表我「先」判斷是不是前面有星號 有星號的話就把正行變為s
	// temporary 意思就是先把我要的內容丟進去 再把這個內容丟進去我要的地方

	while (!inc17.eof()) {

		getline(inc17, s);
		examination = s[0];
		if ((examination == ' ') && (examination != '*')) {
			struct box box1;
			stringstream ss(s);  // 這句話的意思是說我把第一個字根第二個字做切割 然後先把第一個字丟到ss暫存器裡面 接下來再把第二個字丟到暫存器裡面

			ss >> address_tw;                       // 接下來會先做第一行 ex 先把1這個東西丟到address的暫存器  
			box1.address = address_tw;	               //再把address這個暫存器丟到struct 的address
			ss >> name_tw;		                               // 以此類推
			box1.name = name_tw;
			ss >> type_tw;
			box1.type = type_tw;
			if (box1.type == "from") {
				ss >> gate_from_tw;
				box1.gate_from = gate_from_tw;
			}
			else {
				ss >> fanout_tw;
				box1.fanout = fanout_tw;
				ss >> fanin_tw;
				box1.fanin = fanin_tw;
			}
			if (box1.fanin != 0 && box1.fanin != -1) {
				getline(inc17, s);
				stringstream ss(s);

				int z = 0;
				while (z < box1.fanin) {
					ss >> temporary;
					box1.fanout_number.push_back(temporary);
					z = z + 1;
				}
			}
			databox.push_back(box1);
		}
	}

	// printDataBox(databox);
	vector<string>modeladdress;
	for (int e = 0; e < databox.size(); e++) {
		if ((databox[e].fanin == 0) || (databox[e].fanout == 0)) {
			modeladdress.push_back(databox[e].address);
		}
	}
	// 收集input的wire address
	vector<string> inputWireAddress;
	for (int k = 0; k < databox.size(); k++) {
		if (databox[k].fanin == 0) {
			inputWireAddress.push_back(databox[k].address);
		}
	}
	



	// 把verilog檔案中的input那行做出來
	string inputLine = "input ";
	string inputsave = "";

	for (int i = 0; i < inputWireAddress.size(); i++) {//我先把我的address的數字找出來
		inputsave = inputsave + "gat" + inputWireAddress[i] + ", ";
		inputLine = inputLine + "gat" + inputWireAddress[i];  // inputline="gat"+address數字的number
		if (i < inputWireAddress.size() - 1)  inputLine = inputLine + ", ";  // 要判斷是不是最後一個元素 是最後一個元素就要加上分號
		else  inputLine = inputLine + ";\n";
	}

	vector<string> outputWireAddress;
	for (int a = 0; a < databox.size(); a++) {
		if (databox[a].fanout == 0) {
			outputWireAddress.push_back(databox[a].address);
		}
	}

	string outputLine = "output ";
	string outputsave = "";
	for (int b = 0; b < outputWireAddress.size(); b++) {
		outputsave = outputsave + "gat_out" + outputWireAddress[b];
		outputLine = outputLine + "gat_out" + outputWireAddress[b];
		if (b < outputWireAddress.size() - 1){
			outputLine = outputLine + ", ";
			outputsave = outputsave + ", ";
		}else{
			outputLine = outputLine + ";\n";
			outputsave = outputsave + ");\n";
		}
	}		

	
	string modelline = "model ";
	for (int f = 0; f < modeladdress.size(); f++) {
		string modelLine = "module c17 (" + databox[f].address + ", ";
		if (f < modeladdress.size()) modelline = modelline + ", ";
		else modelline = modelline + ";\n";
	}

	vector<string> WireAddress;
	for (int c = 0; c < databox.size(); c++) {
		if ((databox[c].fanout > 0) && (databox[c].fanin > 0)) {
			WireAddress.push_back(databox[c].address);
		}
	}

	string WireLine = "wire ";
	for (int d = 0; d < WireAddress.size(); d++) {
		WireLine = WireLine + "gat_out" + WireAddress[d];
		if (d < WireAddress.size() - 1) WireLine = WireLine + ", ";
		else WireLine = WireLine + ";\n";
	}

	vector<string> gateLines;
	for (int k = 0; k < databox.size(); k++) {
		if (databox[k].type == "inpt" || databox[k].type == "from") continue;
		if (databox[k].type == "buff")  databox[k].type = "buf";
		string newLine = databox[k].type + " gat" + databox[k].address
			+ " (gat_out" + databox[k].address + ", ";
		for (int i = 0; i < databox[k].fanout_number.size(); i++) {
			// 找address=i的資料在databox的哪裡
			string targetString = databox[k].fanout_number[i];
			int targetIndex;
			for (int j = 0; j < databox.size(); j++) {
				if (databox[j].address == targetString) {
					targetIndex = j;
					break;
				}
			}
			while (databox[targetIndex].type == "from") {
				targetString = databox[targetIndex].gate_from;
				for (int l = 0; l < databox.size(); l++) {
					if (databox[l].name == targetString) {
						targetIndex = l;
						break;
					}
				}
			}
			if (databox[targetIndex].fanout_number.size() > 0)       //假如我的targetindex的vector>0的話就會印出gat_out
				newLine = newLine + "gat_out" + databox[targetIndex].address;
			else
			newLine = newLine + "gat" + databox[targetIndex].address;
			if (i < databox[k].fanout_number.size() - 1) newLine = newLine + ", ";
			else newLine = newLine + ");\n";
		}
		gateLines.push_back(newLine);
	}

	{
		//cout << " `timescale 1ns / 1ps " <<"\n" <<
		//"module c17 (" << inputsave<<outputsave << inputLine << outputLine << WireLine;
		outc17<< " `timescale 1ns / 1ps " << "\n" <<
		"module " << module_name << " (" << inputsave << outputsave << inputLine << outputLine << WireLine;

	}
	for (int k = 0; k < gateLines.size(); k++)
	{
		//cout << gateLines[k];
		outc17 << gateLines[k];		
	}
	//cout<<"endmodule\n";
	outc17 << "endmodule\n";
}
