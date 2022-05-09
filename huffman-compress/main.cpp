#define _CRT_SECURE_NO_WARNINGS 1
#pragma once

#include "head.h"
#include "huffman.h"

struct CharInfo {
	char ch;
	long long tot;
	string huffman_code;
	CharInfo(long long tot1=0)
		:ch(0)
		,tot(tot1)
		,huffman_code("")
	{}
	CharInfo operator+(const CharInfo &oo)const {
		return CharInfo(tot + oo.tot);
	}
	bool operator!=(const CharInfo &oo)const {
		return tot != oo.tot;
	}
	bool operator<(const CharInfo &oo)const {
		return tot < oo.tot;
	}
};

void FileCompress();
void FileUncompress();

class HuffmanCompress {
protected:
	CharInfo infos[256];
protected:
	/*
	����huffman����
	*/
	void CreateHuffmanCode(HuffmanTreeNode<CharInfo> *root, string &code) {
		if (root == nullptr)
			return;
		CreateHuffmanCode(root->lson, code + '0');
		CreateHuffmanCode(root->rson, code + '1');
		if (root->lson == NULL&&root->rson == NULL)
			infos[root->weight.ch].huffman_code = code;
	}
public:
	HuffmanCompress() {
		for (size_t i = 0; i < 256; i++)
			infos[i].ch = (char)i;
	}
	string Compress(const char *filename) {
		assert(filename);
		FILE *file_object = fopen(filename, "rb");
		assert(file_object);
		cout << "------    �Ѿ��򿪵ȴ�ѹ�����ļ�   -----" << endl;
		unsigned char ch = fgetc(file_object);
		while (!feof(file_object)) {
			infos[ch].tot++;
			ch = fgetc(file_object);
		}
		cout << "------   ��¼����ÿ���ַ��ĳ��ִ���   ----" << endl;
		CharInfo invalid;
		HuffmanTree<CharInfo> huffman_tree(infos, 256,invalid);
		string code;
		cout << "------       ����huffman��      -------" << endl;
		CreateHuffmanCode(huffman_tree.GetRoot(), code);
		cout << "-------    �Ѿ������huffman����   ----" << endl;
		string compress_file = "D:\\loser\\c-work\\source-file\\huffman-compress\\compress.txt";
		FILE *fout_comprss = fopen(compress_file.c_str(), "wb");
		assert(fout_comprss);
		//��¼λ����ÿ��һ���ֽ����һ��
		size_t pos = 0;
		// ѹ������ַ�
		unsigned char value = 0;
		//�ص��ļ�ͷ
		fseek(file_object, 0, SEEK_SET);
		ch = fgetc(file_object);
		while (!feof(file_object)) {
			string &code = infos[ch].huffman_code;
			for (size_t i = 0; i < code.size(); i++) {
				value <<= 1;
				if (code[i] == '1')
					value |= 1;
				++pos;
				if (pos == 8) {
					fputc(value, fout_comprss);
					value = 0;
					pos = 0;
				}
			}
			ch = fgetc(file_object);
		}
		if (pos) {
			value <<= (8 - pos);
			fputc(value, fout_comprss);
		}
		//�����ļ���¼ѹ����Ϣ
		string config_file = "D:\\loser\\c-work\\source-file\\huffman-compress\\config.txt";
		FILE *fout_config = fopen(config_file.c_str(), "wb");
		assert(fout_config);
		char buff[128];
		string line;
		for (size_t i = 0; i < 256; i++) {
			if (infos[i].tot) {
				line += infos[i].ch;
				line += ",";
                line += _itoa(infos[i].tot, buff, 10);
				line += "\n";
				fwrite(line.c_str(), 1, line.size(), fout_config);
				line.clear();
			}
		}
		fclose(file_object);
		fclose(fout_comprss);
		fclose(fout_config);
		return compress_file;
	}
	string Uncompress(const char *filename) {
		assert(filename);
		string config_filename = "D:\\loser\\c-work\\source-file\\huffman-compress\\config.txt";
		FILE *fin_config = fopen(config_filename.c_str(), "rb");
		assert(fin_config);
		string line;
		unsigned char ch = 0;
		while (ReadLine(fin_config, line)) {
			if (line.empty()) {
				line += '\n';
				continue;
			}
			ch = line[0];
			//�м���һ������
			infos[ch].tot = atoi(line.substr(2).c_str());
			line.clear();
		}
		cout << "�Ѿ����������ļ�" << endl;
		CharInfo invalid;
		//��huffman�����½�����
		HuffmanTree<CharInfo> huffman_tree1(infos, 256, invalid);
		HuffmanTreeNode<CharInfo> *root = huffman_tree1.GetRoot();
		//�ܸ���
		long long chars_tot = root->weight.tot;

		string origin_filename = filename;
		FILE *fout_origin = fopen(origin_filename.c_str(), "wb");
		assert(fout_origin);

		string compress_filename = "D:\\loser\\c-work\\source-file\\huffman-compress\\compress.txt";
		FILE *fin_compress = fopen(compress_filename.c_str(), "rb");
		assert(fin_compress);

		HuffmanTreeNode<CharInfo> *cur = root;
		int pos = 8;
		ch = fgetc(fin_compress);
		while (chars_tot > 0) {
			while (cur) {
				if (cur->lson == NULL && cur->rson == NULL) {
					fputc(cur->weight.ch, fout_origin);
					cur = root;
					chars_tot--;
					if (chars_tot == 0)
						break;
				}
				if (ch&(1 << (pos-1)))
					cur = cur->rson;
				else
					cur = cur->lson;
				pos--;
				if (pos == 0) {
					ch = fgetc(fin_compress);
					pos = 8;
				}
			}
		}
		fclose(fin_config);
		fclose(fin_compress);
		fclose(fout_origin);
		return origin_filename;
	}
	bool ReadLine(FILE *fin, string &line) {
		int ch = fgetc(fin);
		if (ch == EOF)
			return false;
		while (ch != EOF && ch != '\n') {
			line += ch;
			ch = fgetc(fin);
		}
		return true;
	}
	void Display() {
		for (size_t i = 0; i < 256; i++) {
			if (!infos[i].tot)
				continue;
			cout << infos[i].ch << " " << infos[i].tot << " " << infos[i].huffman_code;
			puts("");
		}
	}
};

int main() {
	FileCompress();
	FileUncompress();
	return 0;
}

void FileCompress() {
	HuffmanCompress fc;
	cout << "��ʼѹ��" << endl;
	int kaishi = GetTickCount();
	fc.Compress("D:\\loser\\c-work\\source-file\\huffman-compress\\test.txt");
	int jieshu = GetTickCount();
	cout << "ѹ����ʱ "<<jieshu-kaishi<<endl;
	return;
}

void FileUncompress() {
	HuffmanCompress fc;
	cout << "��ʼ��ѹ" << endl;
	int kaishi = GetTickCount();
	fc.Uncompress("D:\\loser\\c-work\\source-file\\huffman-compress\\test_origin.txt");
	int jieshu = GetTickCount();
	cout << "��ѹ��ʱ" << jieshu - kaishi << endl;
	return;
}