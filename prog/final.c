/*
 ***************************************************************************
 *              ____   ___  ____   ____       _   __   __                  *
 *             |  _ \ |_ _|/ ___| |  _ \     / \  \ \ / /                  *
 *             | | | | | | \___ \ | |_) |   / _ \  \ V /                   *
 *             | |_| | | |  ___) ||  _ <   / ___ \  | |                    *
 *             |____/ |___||____/ |_| \_\ /_/   \_\ |_|   VERSION 1.       *
 *                                                                         *
 * +-----------+---------------------------------------------------------+ *
 * |  File     | final.c                                                 | *
 * +-----------+---------------------------------------------------------+ *
 * |  Version  | 1                                                       | *
 * +-----------+---------------------------------------------------------+ *
 * |  Year     | 2012                                                    | *
 * +-----------+---------------------------------------------------------+ *
 * | Authors   | Afsaneh Mohammadzaheri <afsaneh.mohamadzaheri@gmail.com>| *
 * |           | Hossein Sadeghi <sadeghi@um.ac.ir>                      | *
 * |           | Keyvan Hosseini <k-hosseini@um.ac.ir>                   | *
 * |           | Mahdi Navazandeh <m.navazandeh@gmail.com>               | *
 * +-----------+---------------------------------------------------------+ *
 * |  Copyright (c) 2012 The DISRAY Authors. All rights reserved.        | *
 * +---------------------------------------------------------------------+ *
 * |  DISRAY : A Distributed Ray tracing by Map-Reduce.                  | *
 * |  Make sure the input(source) is set.                                | *
 * +------------------------------------------------------------------ --+ *
 *                                                                         *
 ***************************************************************************
 */

#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<limits.h>


int main()
{
    FILE *in, **in1, **in2, *out1,  *out2, *f;
    char *line1 = NULL;
    size_t len = 0;
    size_t read;
    unsigned int node_id, cont;
    char file_name1[200], file_name2[200],  ppath1[200], ppath2[200];
    char c,  *pch;
    int i, j, line_num, num, nmin, nodeid, lines, phn, phas_num;
    float *epi, *alfa, *time, *r, epi_min;
    int *ps, *ID, *line;

    snprintf(ppath1, 200, "%s/%s", getenv("HOME"),
                                   "DISRAY/config/count");
    in = fopen(ppath1, "r");
    fscanf(in, "%d", &cont);
    fclose(in);
    in1 = (FILE *)calloc(cont, sizeof(FILE));
    in2 = (FILE *)calloc(cont, sizeof(FILE));

    node_id = 0;
    i = 0;
    phn = 0;
    while(node_id < cont) {
       snprintf(file_name1, 200, "%s/%s%d", getenv("HOME"),
                                            "DISRAY/data/reduce",
                                            node_id);

       in1[node_id] = fopen(file_name1, "r");
       fscanf(in1[node_id], "%d\n", &phas_num);
       phn = phn + phas_num;
       snprintf(file_name2, 200, "%s/%s%d", getenv("HOME"),
                                            "DISRAY/data/path",
                                            node_id);

       in2[node_id] = fopen(file_name2,  "r");
       node_id = node_id++;
    }

    epi = (float *)calloc(phn, sizeof(float));
    alfa = (float *)calloc(phn, sizeof(float));
    time = (float *)calloc(phn, sizeof(float));
    r = (float *)calloc(phn, sizeof(float));
    ps = (int *)calloc(phn, sizeof(int));
    line = (int *)calloc(phn, sizeof(int));
    ID = (int *)calloc(phn, sizeof(int));

    node_id = 0;

    while(node_id < cont) {

       line_num = 2;
       while(! feof( in1[node_id])) {
	       fscanf(in1[node_id], "%f\t%f\t%f\t%f\t\%d\n", &epi[i],
                                                         &alfa[i],
                                                         &time[i],
                                                         &r[i],
                                                         &ps[i]);
	       ID[i] = node_id;
	       line[i] = line_num;
	       line_num = line_num++;
	       i = i++;
       }

       fclose(in1[node_id]);
       node_id = node_id++;
    }

    num = i;
    snprintf(ppath1, 200, "%s/%s", getenv("HOME"), 
                                   "DISRAY/data/out1");

    out1 = fopen(ppath1, "w");
    snprintf(ppath2, 200, "%s/%s", getenv("HOME"), 
                                   "DISRAY/data/out2");

    out2 = fopen(ppath2, "w");
    j = 0;
    while(j < num) {

        epi_min = 100000;
        i = 0;
        for(i = 0; i < num; i++) {
            if(epi[i] <= epi_min) {
                epi_min = epi[i];
                nmin = i;
            }
        }

        fprintf(out1, "%.3f\t%.3f\t%.3f\t%.3f\t%d\n", epi[nmin],
                                                      alfa[nmin],
                                                      time[nmin],
                                                      r[nmin],
                                                      ps[nmin]);
        epi[nmin] = 200000;
        nodeid = ID[nmin];
        fseek(in2[nodeid], 0, SEEK_SET);
        lines = 1; 
        c = '\n';

        while((c = fgetc(in2[nodeid])) != EOF) {
            if(lines == line[nmin]) 
                break;
            if(c == '\n') {
                do{
                    c = fgetc(in2[nodeid]);
                } while(c == '\n');

                lines = lines++;
            } /*if(c=='\n')*/

       } /*((c=fgetc(in2[nodeid]))!=EOF)*/

       read = getline(&line1,  &len, in2[nodeid]);
       fputc(c, out2);

       fprintf(out2, "%s", line1);

       j = j++;
    } /*while(j<num)*/

    for(j = 0; j < cont; j++) {
        fclose(in2[j]);
    }

    fclose(out1);
    fclose(out2);
    free(line1);
    exit(EXIT_SUCCESS);
    printf("end of program\n");

    return 0;
}

