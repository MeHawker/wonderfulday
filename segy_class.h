#include<iostream>
#include<cstring>
#include<fstream>
#include<tchar.h>
#include<vector>

using namespace std;


struct Binary_file_head_400
{
	int jobid;  /* 3201 - 3204  job identification number */
	int lino;   /* 3205 - 3208  line number (only one line per reel) */
	int reno;   /* 3209 - 3212  reel number */
	short int ntrpr;  /* 3213 - 3214  number of data traces per record */
	short int nart;   /* 3215 - 3216  number of auxiliary traces per record */
	short int hdt;    /* 3217 - 3218  sample interval in micro secs for this reel */
	short int dto;    /* 3219 - 3220  same for original field recording */
	unsigned short int hns;    /* 3221 - 3222  number of samples per trace for this reel */
	unsigned short int nso;    /* 3223 - 3224  same for original field recording */
	short int format; /* 3225 - 3226  data sample format code:
								  1 = floating point (4 bytes)
								  2 = fixed point (4 bytes)
								  3 = fixed point (2 bytes)
								  4 = fixed point w/gain code (4 bytes) */
	short int fold;   /* 3227 - 3228  CDP fold expected per CDP ensemble */
	short int tsort;  /* 3229 - 3230  trace sorting code:
								  1 = as recorded (no sorting)
								  2 = CDP ensemble
								  3 = single fold continuous profile
								  4 = horizontally stacked */
	short int vscode; /* 3231 - 3232  vertical sum code:
								  1 = no sum
								  2 = two sum ...
								  N = N sum (N = 32,767) */
	short int hsfs;   /* 3233 - 3234  sweep frequency at start */
	short int hsfe;   /* 3235 - 3236  sweep frequency at end */
	short int hslen;  /* 3237 - 3238  sweep length (ms) */
	short int hstyp;  /* 3239 - 3240  sweep type code:
								  1 = linear
								  2 = parabolic
								  3 = exponential
								  4 = other */
	short int schn;   /* 3241 - 3242  trace number of sweep channel */
	short int hstas;  /* 3243 - 3244  sweep trace taper length at start if
								  tapered (the taper starts at zero time
								  and is effective for this length) */
	short int hstae;  /* 3245 - 3246  sweep trace taper length at end (the ending
								  taper starts at sweep length minus the taper
								  length at end) */
	short int htatyp; /* 3247 - 3248  sweep trace taper type code:
								  1 = linear
								  2 = cos-squared
								  3 = other */
	short int hcorr;  /* 3249 - 3250  correlated data traces code:
								  1 = no
								  2 = yes */
	short int bgrcv;  /* 3251 - 3252  binary gain recovered code:
								  1 = yes
								  2 = no */
	short int rcvm;   /* 3253 - 3254  amplitude recovery method code:
								  1 = none
								  2 = spherical divergence
								  3 = AGC
								  4 = other */
	short int mfeet;  /* 3255 - 3256  measurement system code:
								  1 = meters
								  2 = feet */
	short int polyt;  /* 3257 - 3258  impulse signal polarity code:
								  1 = increase in pressure or upward
									  geophone case movement gives
									  negative number on tape
								  2 = increase in pressure or upward
									  geophone case movement gives
									  positive number on tape */
	short int vpol;   /* 3259 - 3260  vibratory polarity code:
								  code seismic signal lags pilot by
								  1 337.5 to  22.5 degrees
								  2  22.5 to  67.5 degrees
								  3  67.5 to 112.5 degrees
								  4 112.5 to 157.5 degrees
								  5 157.5 to 202.5 degrees
								  6 202.5 to 247.5 degrees
								  7 247.5 to 292.5 degrees
								  8 293.5 to 337.5 degrees */
	short int hunass[170]; /* 3261 - 3600  unassigned */
};

struct traceheader
{
	int trace_seq_global;
	int trace_seq_local;
	int ori_record_num;
	int trace_num_field;
	int source_point;
	int ensemble_num;
	int ens_trace_num;
	short int trace_code;
	short int num_vert_sum;
	short int num_horz_sum;
	short int data_use;
	int dist_cent_recv;
	int decv_grp_elev;
	int surf_elev_src;
	int soutce_depth;
	int detum_elev_recv;
	int datum_elav_src;
	int water_depth_src;
	int water_depth_grp;
	short int scale_depth;
	short int scale_coor;
	int src_coor_x;
	int src_coor_y;
	int grp_coor_x;
	int grp_coor_y;
	short int coor_unit;
	short int weather_vel;
	short int sweather_vel;
	short int uphole_t_src;
	short int uphole_t_grp;
	short int src_sta_cerrc;
	short int grp_sta_corrc;
	short int total_sta;
	short int lag_time_a;
	short int lag_time_b;
	short int delay_t;
	short int mute_t_strt;
	short int mute_t_end;
	unsigned short int num_of_sampl;
	unsigned short int sample_intrvl;
	short int gain_type;
	short int gain_const;
	short int gain_init;
	short int corrltd;
	short int sweep_freq_start;
	short int sweep_freq_end;
	short int sweep_length;
	short int sweep_type;
	short int sweep_taper_len_start;
	short int sweep_taper_len_end;
	short int taper_type;
	short int alias_freq;
	short int alias_slope;
	short int notch_freq;
	short int notch_slope;
	short int lowcut_freq;
	short int highcut_freq;
	short int lowcut_slope;
	short int higncut_slope;
	short int year;
	short int day;
	short int hour;
	short int minute;
	short int second;
	short int time_code;
	short int weight_fact;
	short int geophne_roll;
	short int geophne_trace;
	short int geophne_last;
	short int gap_size;
	short int over_travel;
	int ens_coor_x;
	int ens_coor_y;
	int inline_;
	int cross;
	int shootpoint;
	short int shootpoint_scale;
	short int trace_unit;
	char transd_const[6];
	short int transd_unit;
	short int scale_time;
	short int src_orient;
	char src_direction[6];
	char src_measuremt[6];
	short int src_unit;
	char unnassigned[6];
};

static const unsigned char eTOa[256] = {
		  0,  1,  2,  3,156,  9,134,127,151,141,142, 11, 12, 13, 14, 15,
		 16, 17, 18, 19,157,133,  8,135, 24, 25,146,143, 28, 29, 30, 31,
		128,129,130,131,132, 10, 23, 27,136,137,138,139,140,  5,  6,  7,
		144,145, 22,147,148,149,150,  4,152,153,154,155, 20, 21,158, 26,
		 32,160,161,162,163,164,165,166,167,168, 91, 46, 60, 40, 43, 33,
		 38,169,170,171,172,173,174,175,176,177, 93, 36, 42, 41, 59, 94,
		 45, 47,178,179,180,181,182,183,184,185,124, 44, 37, 95, 62, 63,
		186,187,188,189,190,191,192,193,194, 96, 58, 35, 64, 39, 61, 34,
		195, 97, 98, 99,100,101,102,103,104,105,196,197,198,199,200,201,
		202,106,107,108,109,110,111,112,113,114,203,204,205,206,207,208,
		209,126,115,116,117,118,119,120,121,122,210,211,212,213,214,215,
		216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,
		123, 65, 66, 67, 68, 69, 70, 71, 72, 73,232,233,234,235,236,237,
		125, 74, 75, 76, 77, 78, 79, 80, 81, 82,238,239,240,241,242,243,
		 92,159, 83, 84, 85, 86, 87, 88, 89, 90,244,245,246,247,248,249,
		 48, 49, 50, 51, 52, 53, 54, 55, 56, 57,250,251,252,253,254,255
};
class segy
{
public:
	void openfile();
	void savefile();
	void crossfile();
	void get_all_trace();
	void read_all_trace();
	void set_trace();
	void write_file();
	void print_trace_header();
	void print_text_header();
	void print_binary_header();
	void print_trace_data(int, int, float**);
	void getbuf(const char* const, char* const&, const int &offset, const int &len);
	void switchbyte(char* const c, const int &len);

	unsigned char tfile_header_[3200];
	unsigned char extfile_header_[3200];
	Binary_file_head_400 bfilehead_;
	traceheader trace_header_;
	float **trace_;//data matrix
	int file_length_;
	int trace_num_;
	int sample_num_;
	
	string filename_;
	fstream in_;
private:
	int swap(int);
	unsigned int swap(unsigned int);
	unsigned short int swap(unsigned short int);
	short int swap(short int);
	float ibmTOieee(uint32_t ibm);
};

