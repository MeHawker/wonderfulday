#define _CRT_SECURE_NO_WARNINGS
#include "segy_class.h"
#include<iostream>

unsigned short int segy::swap(unsigned short int a)
{
	unsigned short int temp1 = (a >> 8);
	return (a << 8) | temp1;
}

short int segy::swap(short int a)
{
	short int temp1 = a >> 8;
	return (a << 8) | temp1;
}

int segy::swap(int a)
{
	unsigned short int temp1 = (a >> 16);
	unsigned short int temp2 = (a & 0x0000FFFF);
	temp1= swap(temp1);
	temp2= swap(temp2);
	int b = (int)temp2;
	b = b << 16;
	b = b | (int)temp1;

	return b;
}

unsigned int segy::swap(unsigned int a)
{
	unsigned short int temp1 = (a >> 16);
	unsigned short int temp2 = (a & 0x0000FFFF);
	temp1 = swap(temp1);
	temp2 = swap(temp2);

	unsigned int b = (unsigned int)temp2;
	b = b << 16;
	return b | (unsigned int)temp1;
}

float segy::ibmTOieee(uint32_t ibm)
{
	//ibm =ntohl(ibm);
	uint32_t fraction;
	int32_t exponent;
	int32_t sign;

	fraction = ibm;
	sign = (int32_t)(ibm >> 31);
	fraction <<= 1;
	exponent = (int32_t)(fraction >> 25);
	fraction <<= 7;

	if (fraction == 0)
	{
		exponent = 0;
		goto done;
	}
	exponent = (exponent << 2) - 130;

	while (fraction < 0x80000000)
	{
		exponent--;
		fraction <<= 1;
	}
	if (exponent <= 0)
	{
		if (exponent < -24) fraction = 0;
		else fraction >>= -exponent;
		exponent = 0;
	}
	else if (exponent >= 255)
	{
		fraction = 0;
		exponent = 255;
	}
	else fraction <<= 1;

done:
	uint32_t ieee=((uint32_t)(fraction >> 9)) | ((uint32_t)(exponent << 23)) | ((uint32_t)(sign << 31));
	float result;
	memcpy(&result, &ieee, sizeof(float));
	return result;
}

void segy::openfile()
{
	char temp1[4000];
	char s1[100];
	cout << "please input the name of segy:" << endl;
	cin >> s1;
	
	ifstream in_(s1,ios::binary);
	if (in_.fail())
	{
		cout << "error opening file:" << s1 << endl;
		system("pause");
		exit(0);
	}
	in_.seekg(0, in_.end);
	file_length_ = in_.tellg();
	cout << file_length_ << endl;
	in_.seekg(0, in_.beg);
	memset(tfile_header_, '\0', 3200 * sizeof(char));
	in_.read((char*)tfile_header_, sizeof(tfile_header_));

	in_.read(temp1, sizeof(bfilehead_));
	memcpy(&bfilehead_, temp1, sizeof(bfilehead_));

	sample_num_ = swap(bfilehead_.hns);
	trace_num_ = (file_length_ - 3600) / (240 + 4 * sample_num_);
	cout << "the number of trace is:" << trace_num_ << endl;
	cout << "the number of sample of each trace is:" << sample_num_ << endl;
	trace_ = new float*[trace_num_];
	for (int i = 0; i < trace_num_; i++)
	{
		trace_[i] = new float[sample_num_];
	}
	in_.read(temp1, sizeof(trace_header_));
	memcpy(&trace_header_, temp1, sizeof(trace_header_));
	int cal_trace = 0;
	
	while (cal_trace < trace_num_)
	{
		if (swap(bfilehead_.format) == 5)//ieee
		{
			if (cal_trace)
			{
				in_.seekg(240, in_.cur);
			}
			
			char*buffer(nullptr);
			buffer = new char[sample_num_ * sizeof(float)];
			in_.read(buffer, sizeof(buffer));
			int index;
			char intTOchar[4];
			for (index = 0; index < sample_num_; index++)
			{
				getbuf(buffer, intTOchar, index * 4, 4);
				memcpy(&trace_[cal_trace][index], intTOchar, 4);
				cout << trace_[cal_trace][index] << endl;
			}
			delete []buffer;
		}
		else if (swap(bfilehead_.format) == 1)//ibm
		{
			if (cal_trace)
			{
				in_.seekg(240, in_.cur);
			}
			char *buffer = new char[sample_num_ * sizeof(float)];
			in_.read(buffer, sample_num_ * sizeof(float));
			int index; uint32_t temp2;
			char intTOchar[4];
			for (index = 0; index < sample_num_; index++)
			{
				getbuf(buffer, intTOchar, index * 4, 4);
				memcpy(&temp2, intTOchar, 4);
				trace_[cal_trace][index] = ibmTOieee(temp2);
			}
			delete[]buffer;
		}
		cal_trace++;
	}
	in_.close();

}

void segy::print_trace_header()
{
	cout << "trace_seq_global:" << swap(trace_header_.trace_seq_global) << endl;
	cout << "trace_seq_local:" << swap(trace_header_.trace_seq_local) << endl;
	cout << "ori_record_num:" << swap(trace_header_.ori_record_num) << endl;
	cout << "trace_num_field:" << swap(trace_header_.trace_num_field) << endl;
	cout << "source_point:" << swap(trace_header_.source_point) << endl;
	cout << "ensemble_num:" << swap(trace_header_.ensemble_num) << endl;
	cout << "ens_trace_num:" << swap(trace_header_.ens_trace_num) << endl;
	cout << "trace_code:" << swap(trace_header_.trace_code) << endl;
	cout << "num_vert_sum:" << swap(trace_header_.num_vert_sum) << endl;
	cout << "num_horz_sum:" << swap(trace_header_.num_horz_sum) << endl;
	cout << "data_use:" << swap(trace_header_.data_use) << endl;
	cout << "dist_cent_recv:" << swap(trace_header_.dist_cent_recv) << endl;
	cout << "decv_grp_elev:" << swap(trace_header_.decv_grp_elev) << endl;
	cout << "surf_elev_src:" << swap(trace_header_.surf_elev_src) << endl;
	cout << "soutce_depth:" << swap(trace_header_.soutce_depth) << endl;
	cout << "detum_elev_recv:" << swap(trace_header_.detum_elev_recv) << endl;
	cout << "datum_elav_src:" << swap(trace_header_.datum_elav_src) << endl;
	cout << "water_depth_src:" << swap(trace_header_.water_depth_src) << endl;
	cout << "water_depth_grp:" << swap(trace_header_.water_depth_grp) << endl;
	cout << "scale_depth:" << swap(trace_header_.scale_depth) << endl;
	cout << "scale_coor:" << swap(trace_header_.scale_coor) << endl;
	cout << "src_coor_x:" << swap(trace_header_.src_coor_x )<< endl;
	cout << "src_coor_y:" << swap(trace_header_.src_coor_y) << endl;
	cout << "grp_coor_x:" << swap(trace_header_.grp_coor_x) << endl;
	cout << "grp_coor_y:" << swap(trace_header_.grp_coor_y) << endl;
	cout << "coor_unit:" << swap(trace_header_.coor_unit) << endl;
	cout << "weather_vel:" << swap(trace_header_.weather_vel) << endl;
	cout << "sweather_vel:" << swap(trace_header_.sweather_vel) << endl;
	cout << "uphole_t_src:" << swap(trace_header_.uphole_t_src) << endl;
	cout << "uphole_t_grp:" << swap(trace_header_.uphole_t_grp) << endl;
	cout << "src_sta_cerrc:" << swap(trace_header_.src_sta_cerrc) << endl;
	cout << "grp_sta_corrc:" << swap(trace_header_.grp_sta_corrc) << endl;
	cout << "total_sta:" << swap(trace_header_.total_sta) << endl;
	cout << "lag_time_a:" << swap(trace_header_.lag_time_a) << endl;
	cout << "lag_time_b:" << swap(trace_header_.lag_time_b )<< endl;
	cout << "delay_t:" << swap(trace_header_.delay_t) << endl;
	cout << "mute_t_strt:" << swap(trace_header_.mute_t_strt) << endl;
	cout << "mute_t_end:" << swap(trace_header_.mute_t_end) << endl;
	cout << "num_of_sampl:" << swap(trace_header_.num_of_sampl) << endl;
	cout << "sample_intrvl:" << swap(trace_header_.sample_intrvl) << endl;
	cout << "gain_type:" << swap(trace_header_.gain_type) << endl;
	cout << "gain_const:" << swap(trace_header_.gain_const) << endl;
	cout << "gain_init:" << swap(trace_header_.gain_init) << endl;
	cout << "corrltd:" << swap(trace_header_.corrltd) << endl;
	cout << "sweep_freq_start:" << swap(trace_header_.sweep_freq_start) << endl;
	cout << "sweep_freq_end:" << swap(trace_header_.sweep_freq_end) << endl;
	cout << "sweep_length:" << swap(trace_header_.sweep_length) << endl;
	cout << "sweep_type:" << swap(trace_header_.sweep_type) << endl;
	cout << "sweep_taper_len_start:" << swap(trace_header_.sweep_taper_len_start) << endl;
	cout << "sweep_taper_len_end:" << swap(trace_header_.sweep_taper_len_end) << endl;
	cout << "taper_type:" << swap(trace_header_.taper_type) << endl;
	cout << "alias_freq:" << swap(trace_header_.alias_freq) << endl;
	cout << "alias_slope:" << swap(trace_header_.alias_slope) << endl;
	cout << "notch_freq:" << swap(trace_header_.notch_freq )<< endl;
	cout << "notch_slope:" << swap(trace_header_.notch_slope) << endl;
	cout << "lowcut_freq:" << swap(trace_header_.lowcut_freq) << endl;
	cout << "highcut_freq:" << swap(trace_header_.highcut_freq) << endl;
	cout << "lowcut_slope:" << swap(trace_header_.lowcut_slope) << endl;
	cout << "higncut_slope:" << swap(trace_header_.higncut_slope )<< endl;
	cout << "year:" << swap(trace_header_.year )<< endl;
	cout << "day:" << swap(trace_header_.day )<< endl;
	cout << "hour:" << swap(trace_header_.hour )<< endl;
	cout << "minute:" << swap(trace_header_.minute )<< endl;
	cout << "second:" << swap(trace_header_.second )<< endl;
	cout << "time_code:" << swap(trace_header_.time_code) << endl;
	cout << "weight_fact:" << swap(trace_header_.weight_fact) << endl;
	cout << "geophne_roll:" << swap(trace_header_.geophne_roll) << endl;
	cout << "geophne_trace:" << swap(trace_header_.geophne_trace) << endl;
	cout << "geophne_last:" << swap(trace_header_.geophne_last) << endl;
	cout << "gap_size:" << swap(trace_header_.gap_size) << endl;
	cout << "over_travel:" << swap(trace_header_.over_travel) << endl;
	cout << "ens_coor_x:" << swap(trace_header_.ens_coor_x) << endl;
	cout << "ens_coor_y:" << swap(trace_header_.ens_coor_y) << endl;
	cout << "inline_:" << swap(trace_header_.inline_) << endl;
	cout << "cross:" << swap(trace_header_.cross) << endl;
	cout << "shootpoint:" << swap(trace_header_.shootpoint) << endl;
	cout << "trace_unit:" << swap(trace_header_.trace_unit) << endl;
	for (int i = 0; i < 6; i++)
	{
		cout << "transd_const" << i << ":" << swap(trace_header_.transd_const[i]) << " ";
	}
	cout << endl;
	cout << "transd_unit:" << swap(trace_header_.transd_unit) << endl;
	cout << "scale_time:" << swap(trace_header_.scale_time) << endl;
	cout << "src_orient:" << swap(trace_header_.src_orient) << endl;
	for (int i = 0; i < 6; i++)
	{
		cout << "src_direction" << i << ":" << swap(trace_header_.src_direction[i]) << " ";
	}
	cout << endl;
	for (int i = 0; i < 6; i++)
	{
		cout << "src_measuremt" << i << ":" << swap(trace_header_.src_measuremt[i]) << " ";
	}
	cout << endl;
	cout << "src_unit:" << swap(trace_header_.src_unit) << endl;
	for (int i = 0; i < 6; i++)
	{
		cout << "unnassigned" << i << ":" << swap(trace_header_.unnassigned[i]) << " ";
	}
	cout << endl;
}

void segy::print_binary_header()
{
	cout << "jobid:" << swap(bfilehead_.jobid) << endl;
	cout << "lino:" << swap(bfilehead_.lino) << endl;
	cout << "reno:" << swap(bfilehead_.reno) << endl;
	cout << "ntrpr:" << swap(bfilehead_.ntrpr) << endl;
	cout << "nart:" << swap(bfilehead_.nart) << endl;
	cout << "hdt:" << swap(bfilehead_.hdt) << endl;
	cout << "dto:" << swap(bfilehead_.dto) << endl;
	cout << "hns:" << swap(bfilehead_.hns) << endl;
	cout << "nso:" << swap(bfilehead_.nso) << endl;
	cout << "format:" << swap(bfilehead_.format) << endl;
	cout << "fold:" << swap(bfilehead_.fold) << endl;
	cout << "tsort:" << swap(bfilehead_.tsort) << endl;
	cout << "vscode:" << swap(bfilehead_.vscode) << endl;
	cout << "hsfs:" << swap(bfilehead_.hsfs) << endl;
	cout << "hsfe:" << swap(bfilehead_.hsfe) << endl;
	cout << "hslen:" << swap(bfilehead_.hslen) << endl;
	cout << "hstyp:" << swap(bfilehead_.hstyp) << endl;
	cout << "schn:" << swap(bfilehead_.schn) << endl;
	cout << "hstas:" << swap(bfilehead_.hstas) << endl;
	cout << "hstae:" << swap(bfilehead_.hstae) << endl;
	cout << "htatyp:" << swap(bfilehead_.htatyp) << endl;
	cout << "hcorr:" << swap(bfilehead_.hcorr) << endl;
	cout << "bgrcv:" << swap(bfilehead_.bgrcv) << endl;
	cout << "rcvm:" << swap(bfilehead_.rcvm) << endl;
	cout << "mfeet:" << swap(bfilehead_.mfeet) << endl;
	cout << "polyt:" << swap(bfilehead_.polyt) << endl;
	cout << "vpol:" << swap(bfilehead_.vpol) << endl;
	for (int i = 0; i < 170; i++)
	{
		cout << "hunass" <<i<<":"<< swap(bfilehead_.hunass[i]) << " ";
	}
	cout << endl;
	
	

}

void segy::print_text_header()
{
	for (size_t i = 0; i < 3200; i++)
	{
		if ((i % 80) == 0)   //80字节一组
			std::cout << std::endl;

		std::cout << eTOa[(int)(tfile_header_[i])];//读表法

	}
	std::cout << std::endl;
}

void segy::write_file()
{
	char f2[50];
	cout << "please input the name you want to save the seismic matrix:(binary output)" << endl;
	cin >> f2;
	ofstream outfile1(f2, ios::binary);
	if (!outfile1)
	{
		cerr << "open f2.bin error!" << endl;
	}
	for (int i = 0; i < trace_num_; i++)
	{
		for (int j = 0; j < sample_num_; j++)
		{
			outfile1.write((char*)(&trace_[i][j]), sizeof(float));
		}
	}
	outfile1.close();
}

void segy::getbuf(const char* const src, char* const &buffer, const int &offset, const int &len)
{
	int index;
	for (index = 0; index < len; index++)
		buffer[index] = src[offset + index];
	switchbyte(buffer, len);
	return;
}

void segy::switchbyte(char* const c, const int &len)  //工作站的大端模式变成PC小端模式
{
	int index; char tmp;
	for (index = 0; index < len / 2; index++)
	{
		tmp = c[index];
		c[index] = c[len - index - 1];
		c[len - index - 1] = tmp;
	}
}
