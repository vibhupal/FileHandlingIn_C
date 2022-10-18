#ifndef HEADER_H
#define HEADER_H

typedef struct exam_center{
	char exam_center_no[7];
	char exam_id[7];
	char college[20];
	char addr[50];
	struct exam_center *next;
}exam_center;

exam_center *head;
typedef struct candidate_data{
	char candidate_id[5];
	char name[15];
	char exam_id[7];
	char start_date[11];
	char end_date[11];
	exam_center *center;
	struct candidate_data *nxt;
} data;

data *start;

#endif
