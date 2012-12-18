/*
 ***************************************************************************
 *              ____   ___  ____   ____       _   __   __                  *
 *             |  _ \ |_ _|/ ___| |  _ \     / \  \ \ / /                  *
 *             | | | | | | \___ \ | |_) |   / _ \  \ V /                   *
 *             | |_| | | |  ___) ||  _ <   / ___ \  | |                    *
 *             |____/ |___||____/ |_| \_\ /_/   \_\ |_|   VERSION 1.       *
 *                                                                         *
 * +-----------+---------------------------------------------------------+ *
 * |  File     | reduce.c                                                | *
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
#include<string.h>
#include<stdlib.h>
#include<limits.h>


FILE *f, *out2;
int rayout(int, int, float, float, float);

int main()
{
    FILE *in, *out;
    float epi, alfa, time, zzt, r, dis, a;
    float *tp, *tpkp, *tpcp, *tpcs, *tP, *tPn;
    float *ts, *tS, *tSn, *tscs, *tscp;
    float *tpmp, *tpms, *tsms, *tsmp;
    int *np, *ns, *nPn, *nSn, *nS, *nP;
    int *npcp, *nscs, *npcs, *nscp, *npkp;
    int *npmp, *npms, *nsmp, *nsms;
    int i, j, num, ps, line, mm, node_id, num_phas;
    float angle;
    char path1[200];
    char file_name1[200], file_name2[200],
         file_name3[200], file_name4[200];

    dis=1.0;

    snprintf(path1, 200, "%s/%s", getenv("HOME"),
                                  "DISRAY/config/id");
    in = fopen(path1, "r");
    fscanf(in, "%d", &node_id);
    fclose(in);

    snprintf(path1, 200, "%s/%s", getenv("HOME"),
                                  "DISRAY/data/source");
    in = fopen(path1, "r");
    fscanf(in, "%f\n%f\n%f", &zzt, &a, &dis);
    fclose(in);
    snprintf(file_name1, 200, "%s/%s%d", getenv("HOME"),
                                        "DISRAY/data/map",
                                        node_id);
    in = fopen(file_name1, "r");

    snprintf(file_name2, 200, "%s/%s%d", getenv("HOME"),
                                         "DISRAY/data/raypath",
                                          node_id);

    snprintf(file_name3, 200, "%s/%s%d", getenv("HOME"),
                                        "DISRAY/data/reduce",
                                        node_id);
    out = fopen(file_name3, "w");

    snprintf(file_name4, 200, "%s/%s%d", getenv("HOME"),
                                        "DISRAY/data/path",
                                        node_id);
    out2 = fopen(file_name4, "w");

    mm = 180.0 / dis;
    tp=(float *)calloc(mm, sizeof(float));
    tP=(float *)calloc(mm, sizeof(float));
    tPn=(float *)calloc(mm, sizeof(float));
    tpmp=(float *)calloc(mm, sizeof(float));
    tpms=(float *)calloc(mm, sizeof(float));
    tpcp=(float *)calloc(mm, sizeof(float));
    tpcs=(float *)calloc(mm, sizeof(float));
    tpkp=(float *)calloc(mm, sizeof(float));
    ts=(float *)calloc(mm, sizeof(float));
    tS=(float *)calloc(mm, sizeof(float));
    tSn=(float *)calloc(mm, sizeof(float));
    tsms=(float *)calloc(mm, sizeof(float));
    tsmp=(float *)calloc(mm, sizeof(float));
    tscs=(float *)calloc(mm, sizeof(float));
    tscp=(float *)calloc(mm, sizeof(float));
    tS=(float *)calloc(mm, sizeof(float));
    tP=(float *)calloc(mm, sizeof(float));


    np=(int *)calloc(mm, sizeof(int));
    nP=(int *)calloc(mm, sizeof(int));
    nPn=(int *)calloc(mm, sizeof(int));
    npmp=(int *)calloc(mm, sizeof(int));
    npms=(int *)calloc(mm, sizeof(int));
    npcp=(int *)calloc(mm, sizeof(int));
    npcs=(int *)calloc(mm, sizeof(int));
    npkp=(int *)calloc(mm, sizeof(int));
    ns=(int *)calloc(mm, sizeof(int));
    nS=(int *)calloc(mm, sizeof(int));
    nSn=(int *)calloc(mm, sizeof(int));
    nsms=(int *)calloc(mm, sizeof(int));
    nsmp=(int *)calloc(mm, sizeof(int));
    nscs=(int *)calloc(mm, sizeof(int));
    nscp=(int *)calloc(mm, sizeof(int));
    nS=(int *)calloc(mm, sizeof(int));
    nP=(int *)calloc(mm, sizeof(int));

    for (j = 0; j < mm; j++) {
        tp[j] = 100000.0;  
        ts[j] = 100000.0;
        tPn[j] = 100000.0; 
        tSn[j] = 100000.0;
        tpcp[j] = 100000.0; 
        tscs[j] = 100000.0;
        tscp[j] = 100000.0; 
        tpcs[j] = 100000.0;
        tpmp[j] = 100000.0; 
        tpms[j] = 100000.0;
        tsmp[j] = 100000.0; 
        tsms[j] = 100000.0;
        tP[j] = 100000.0; 
        tS[j] = 100000.0; 
        tpkp[j] = 100000.0;
    } /*for (j = 0; j < mm; j++)*/

    j=1; i=1; num_phas = 0;
    while(!feof(in)) {

        fscanf(in, "%f\t%f\t%f\t%f\t%d\n", &epi,
                                           &alfa,
                                           &time,
                                           &r,
                                           &ps);

        j = 0;
        if(time != 0.0) {
            for(j = 0; j < mm; j++) {

                if((epi - (dis * j)) * 
                   (epi - dis * (j + 1)) <= 
                    0.0) {

                    //crust phases
                    if(r <= 6371.0 && r > 6336.0) {
                        if(ps == 1 ) {
                            if(time <= tp[j]) {
                                if(tp[j] == 100000) 
                                    num_phas=num_phas++;
                                tp[j] = time;
                                np[j] = i;
                                break;

                            /* if(time <= tp[j]) */
                            } else if(time > tp[j]) { 
                                break;
                            }
	                    } else if(ps == 2) {
                            if(time <= ts[j]) {
                                if(ts[j] == 100000) 
                                    num_phas=num_phas++;
                                ts[j] = time;
                                ns[j] = i;
                                break;
                            } else if(time > ts[j]) { 
                                break;
                            }
                        } /* else if(ps == 2) */
                    } /* if(r <= 6371.0 && r > 6336.0) */

                    /*
                     **********************
                     *  uppermost mantle  *
                     **********************
                     */
                    if(r < 6336.0 && r > 6310.0) {
                        if(ps == 1) {
                            if(time <= tPn[j]) {
                                if(tPn[j] == 100000) 
                                    num_phas = num_phas++;
                                tPn[j] = time;
                                nPn[j] = i;
                                break;
                            } else if(time > tPn[j]) {
                                break;
                            }
                        } else if(ps == 2) {
                            if(time <= tSn[j]) {
                                if(tSn[j] == 100000) 
                                    num_phas = num_phas++;
                                tSn[j] = time;
                                nSn[j] = i;
                                break;
                            } else if(time > tSn[j]) {
                                break;
                            }
                        } /*if(ps == 1)*/
                    } /* if(r < 6336.0 && r > 6310.0) */

                    /*
                     **************************
                     *  reflection from moho  *
                     **************************
                     */
                    if(r == 6336.0) {
                        if(ps == 11) {
                            if(time <= tpmp[j]) {
                                if(tpmp[j] == 100000) 
                                    num_phas=num_phas++;
                                tpmp[j] = time;
                                npmp[j] = i;
                                break;
                            } else if(time > tpmp[j]) {
                                break;
                            }
                        } else if(ps == 12) {
                            if(time <= tpms[j]) {
                                if(tpms[j] == 100000) 
                                    num_phas=num_phas++;
                                tpms[j] = time;
                                break;
                            } else if(time > tpms[j]) {
                                break;
                            }
                        } else if(ps == 21) {
                            if(time <= tsmp[j]) {
                                if(tsmp[j] == 100000) 
                                    num_phas=num_phas++;
                                tsmp[j] = time;
                                nsmp[j] = i;
                                break;
                            } else if(time > tsmp[j]) {
                                break;
                            }
                        } else if(ps == 22) {
                            if(time <= tsms[j]) {
                                if(tsms[j] == 100000) 
                                    num_phas=num_phas++;
                                tsms[j] = time;
                                nsms[j] = i;
                                break;
                            } else if(time > tsms[j]) {
                                break;
                            }
                        } /* if(ps == 11) */
                    } /*if(r == 6336.0) */

                    /*
                     *******************
                     *  mantle phases  *
                     *******************
                     */
                    if( r< 6310.0 && r >= 3483.0) {
                        if(ps == 1) {
                            if(time <= tP[j]) {
		                       if(tP[j] == 100000) 
                                    num_phas = num_phas++;
		                       tP[j] = time;
		                       nP[j] = i;
		                       break;
                            } else if(time > tP[j]) {
                                break;
                            }
                        }else if(ps == 2) {
                            if(time <= tS[j]) {
		                       if(tS[j] == 100000) 
                                    num_phas=num_phas++;
		                       tS[j] = time;
		                       nS[j] = i;
		                       break;
                            }else if(time > tS[j]) {
                                break;
                            }
                        }else if(ps == 11) {
                            if(time <= tpcp[j]) {
		                       if(tpcp[j] == 100000) 
                                    num_phas=num_phas++;
		                       tpcp[j] = time;
		                       npcp[j] = i;
		                       break;
                            }else if(time > tpcp[j]) {
                                break;
                            }
                        }else if(ps == 12) {
                            if(time <= tpcs[j]) {
		                      if(tpcs[j] == 100000) 
                                    num_phas=num_phas++;
		                      tpcs[j] = time;
		                      npcs[j] = i;
		                      break;
                            }else if(time > tpcs[j]) {
                                break;
                            }
                        }else if(ps == 21) {
                            if(time <= tscp[j]) {
		                      if(tscp[j] == 100000) 
                                    num_phas=num_phas++;
		                      tscp[j] = time;
		                      nscp[j] = i;
		                      break;
                            }else if(time > tscp[j]) {
                                break;
                            }
                        }else if(ps == 22) {
                            if(time <= tscs[j]) {
		                      if(tscs[j] == 100000) 
                                    num_phas=num_phas++;
		                      tscs[j] = time;
		                      nscs[j] = i;
		                      break;
                            }else if(time > tscs[j]) {
                                break;
                            }
                        } /* if(ps == 1) */
                    } /* if( r< 6310.0 && r >= 3483.0) */

                    /*
                     *****************
                     *  core phases  *
                     *****************
                     */
                    if(r < 3483.0 && r>= 0.0) {
                        if(ps == 1) {
                            if(time <= tpkp[j]) {
		                       if(tpkp[j] == 100000) 
		                            num_phas=num_phas++;
		                       tpkp[j] = time;
		                       npkp[j] = i;
		                       break;
                            }else if(time > tpkp[j]) {
                                break;
                            }
                        } /* if(ps == 1) */
                    } /* if(r < 3483.0 && r>= 0.0) */
                } /*if((epi - (dis * j)) * (epi - dis  ... */
            } /* for(j = 0; j < mm; j++) */
        } /* if(time != 0.0) */

        i=i++;    //line counter

    } /*while(!feof(in)) */

    fprintf(out, "%d\n", num_phas);
    fprintf(out2, "\n");
    num = i;
    i=1;
    fclose(in);

    if((in=fopen(file_name1, "r")) == NULL)
        printf("map is not opend");

    if(( f=fopen(file_name2, "r")) == NULL)
        printf("raypath.dat is not opening!");
    angle=0.0; line=1;
    i=0;
    while(!feof(in)) {
        i=i++;
        fscanf(in, "%f\t%f\t%f\t%f\t%d\n", &epi,
                                           &alfa, 
                                           &time, 
                                           &r, 
                                           &ps);

        if(i == 1) 
            angle=alfa;

        if(angle != alfa) {
            line = line+1;
            angle = alfa;
        }

        j=0;
        for(j = 0; j < mm ;j++) {
            if((epi - (dis*j)) * (epi - dis * (j+1)) <= 0.0) 
                break;
        }

        if(np[j] == i) {
            fprintf(out, "%f\t%f\t%f\t%f\t%d\n", epi,
                                                 alfa,
                                                 time,
                                                 r,
                                                 ps);
            rayout(line, ps, alfa, r, zzt);
            fprintf(out2, "\n");
            continue;
        } else if(ns[j] == i) {
            fprintf(out, "%f\t%f\t%f\t%f\t%d\n", epi,
                                                 alfa,
                                                 time,
                                                 r,
                                                 ps);
            rayout(line, ps, alfa, r, zzt);
            fprintf(out2,  "\n");
            continue;
        } else if(nP[j] == i) {
            fprintf(out, "%f\t%f\t%f\t%f\t%d\n", epi,
                                                 alfa,
                                                 time,
                                                 r,
                                                 ps);
            rayout(line, ps, alfa, r, zzt);
            fprintf(out2,  "\n");
            continue;
        }else if(nS[j] == i) {
            fprintf(out, "%f\t%f\t%f\t%f\t%d\n", epi,
                                                 alfa,
                                                 time,
                                                 r,
                                                 ps);
            rayout(line, ps, alfa, r, zzt);
            fprintf(out2 ,  "\n");
            continue;
        } else if(nPn[j] == i) {
            fprintf(out, "%f\t%f\t%f\t%f\t%d\n", epi,
                                                 alfa,
                                                 time,
                                                 r,
                                                 ps);
            rayout(line, ps, alfa, r, zzt);
            fprintf(out2,  "\n");
            continue;
        } else if(nSn[j] == i) {
            fprintf(out, "%f\t%f\t%f\t%f\t%d\n", epi,
                                                 alfa,
                                                 time,
                                                 r,
                                                 ps);
            rayout(line, ps, alfa, r, zzt);
            fprintf(out2,  "\n");
            continue;
        } else if(npmp[j] == i) {
            fprintf(out, "%f\t%f\t%f\t%f\t%d\n", epi,
                                                 alfa,
                                                 time,
                                                 r,
                                                 ps);
            rayout(line, ps, alfa, r, zzt);
            fprintf(out2 , "\n");
            continue;
        }else if(npms[j] == i) {
            fprintf(out, "%f\t%f\t%f\t%f\t%d\n", epi,
                                                 alfa,
                                                 time,
                                                 r,
                                                 ps);
            rayout(line, ps, alfa, r, zzt);
            fprintf(out2 , "\n");
            continue;
        }else if(nsmp[j] == i) {
            fprintf(out, "%f\t%f\t%f\t%f\t%d\n", 
                                                 epi,
                                                 alfa,
                                                 time,
                                                 r,
                                                 ps);
            rayout(line, ps, alfa, r, zzt);
            fprintf(out2 , "\n");
            continue;
        }else if(nsms[j] == i) {
            fprintf(out, "%f\t%f\t%f\t%f\t%d\n", epi,
                                                 alfa,
                                                 time,
                                                 r,
                                                 ps);
            rayout(line, ps, alfa, r, zzt);
            fprintf(out2, "\n");
            continue;
      
        //cmb phases
        }else if(npcp[j] == i) {
            fprintf(out, "%f\t%f\t%f\t%f\t%d\n", epi,
                                                 alfa,
                                                 time,
                                                 r,
                                                 ps);
            rayout(line, ps, alfa, r, zzt);
            fprintf(out2, "\n");
            continue;
        } else if(npcs[j] == i) {
            fprintf(out, "%f\t%f\t%f\t%f\t%d\n", epi,
                                                 alfa,
                                                 time,
                                                 r,
                                                 ps);
            rayout(line, ps, alfa, r, zzt);
            fprintf(out2,  "\n");
            continue;
        } else if(nscs[j] == i) {
            fprintf(out, "%f\t%f\t%f\t%f\t%d\n", epi,
                                                 alfa,
                                                 time,
                                                 r,
                                                 ps);
            rayout(line, ps, alfa, r, zzt);
            fprintf(out2,  "\n");
            continue;
        } else if(nscp[j] == i) {
            fprintf(out, "%f\t%f\t%f\t%f\t%d\n", epi,
                                                 alfa,
                                                 time,
                                                 r,
                                                 ps);
            rayout(line, ps, alfa, r, zzt);
            fprintf(out2 , "\n");
            continue;

        /*
         *****************
         *  core phases  *
         *****************
         */
        } else if(npkp[j] == i) {
            fprintf(out, "%f\t%f\t%f\t%f\t%d\n", epi,
                                                 alfa,
                                                 time,
                                                 r,
                                                 ps);
            rayout(line, ps, alfa, r, zzt);
            fprintf(out2, "\n");
            continue;
        } /* if(np[j] == i) */

    } /* while(!feof(in)) */
    fclose(out);
    fclose(out2);
    fclose(in);
    fclose(f);
    return 0;
} /* int main() */



/*
 ***********************************************************
 *                     FUNCTION: rayout                    *
 *                                                         *
 *   This function finds the ray path of the ray in        *
 *   "raypath" input and print it in the output "out2".    *
 *   @line_number :the line number of the ray in inputs    *
 *   @ps ; type of phase,                                  *
 *   @alfa: shooting angle,                                *
 *   @r: radius,                                           *
 *   @z: depth.                                            *
 *                                                         *
 ***********************************************************
 */
int rayout(int line_number, int ps, float alfa, float r, float z)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    float rz;
    char c;
    char * pch;
    int lines=1, i=0;
    rz=6371.0-z;
    fseek(f, 0, SEEK_SET);

    while((c=fgetc(f)) != EOF) {
       if(c == '\n') lines++;
       if(lines == line_number) break;
    } //while
    read = getline(&line,  &len,  f);

    if(alfa >= 89.75) {
      fputs(line, out2);
    } else if(alfa < 89.75) {
        pch = strtok(line, ";");
        fputs(pch, out2);
        fputc(',', out2);
        i=0 ;
        while(pch != NULL) {
            i = i++;
            pch = strtok (NULL, ";");

            if(rz <= 6371.0 && rz >= 6336.0) {
                if(r <= 6371.0 && r > 6336.0) {
                    if(i == 1) {
                        fputs(pch, out2);
                        break;
                    }
                } else if(r == 6336.0) {
                    if(ps == 11 || ps == 21) {
                        if(i == 1) {
                            fputs(pch, out2);
                            break;
                        }
                    } else if(ps == 22 || ps == 12) {
                        if(i == 2) {
                            fputs(pch, out2);
                            break;
                        }
                    }
                } else if(r < 6336.0 && r > 3483.0 ) {
                    if(i == 3) {
                        fputs(pch, out2);
                        fputc(',', out2);
                        continue;
                    }
                    if(i == 4) {
                        fputs(pch , out2);
                        break;
                    }
                } else if(r == 3483.0) {
                    if(i == 3) {
                        fputs(pch, out2);
                        fputc(',', out2);
                        continue;
                    }
                    if(ps == 11 || ps == 21) {
                        if(i == 4) {
                            fputs(pch, out2);
                            break;
                        }
                    } else if (ps == 22 || ps == 12) {
                        if(i == 5) {
                            fputs(pch , out2);
                            break;
                        }
                    }
                } else if(r < 3483.0 && r > 1217.0) {
                    if(i == 3) {
                        fputs(pch, out2);
                        fputc(',', out2);
                        continue;
                    }
                    if(i == 6) {
                        fputs(pch, out2);
                        fputc(',', out2);
                        continue;
                    }
                    if(i == 7) {
                        fputs(pch, out2);
                        break;
                    }
                } /* if(r <= 6371.0 && r > 6336.0) */
            /* if(rz <= 6371.0 && rz >= 6336.0) */
            } else if(rz < 6336.0 && rz > 3483.0) {
                if(r < 6336.0 && r > 3483.0) {
                    if(i == 1) {
                        fputs(pch, out2);
                        break;
                    }
                } else if(r == 3483.0) {
                    if(ps == 11 || ps == 21) {
                        if(i == 1) {
                         fputs(pch, out2);
		                  break;
		                }
		            } else if(ps == 22 || ps == 12) {
		                if(i == 2) {
		                    fputs(pch,out2);
		                    break;
		                }
		            }
                } else if(r < 3483.0 && r >= 1217.0) {
                    if(i == 3) {
                        fputs(pch, out2);
                        continue;
                    }
                    if(i == 4) {
                        fputs(pch, out2);
                        break;
                    }
                } /* if(r < 6336.0 && r > 3483.0) */

            } /* if(rz <= 6371.0 && rz >= 6336.0) */
        } /* while(pch != NULL) */
    }  /* if(alfa >= 89.75) */
    return 0;
} /* int rayout(int line_number, int ps, float... */

