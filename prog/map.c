/*
 ***************************************************************************
 *              ____   ___  ____   ____       _   __   __                  *
 *             |  _ \ |_ _|/ ___| |  _ \     / \  \ \ / /                  *
 *             | | | | | | \___ \ | |_) |   / _ \  \ V /                   *
 *             | |_| | | |  ___) ||  _ <   / ___ \  | |                    *
 *             |____/ |___||____/ |_| \_\ /_/   \_\ |_|   VERSION 1.       *
 *                                                                         *
 * +-----------+---------------------------------------------------------+ *
 * |  File     | map.c                                                   | *
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
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define _PI 3.141592

FILE *out, *out2;

float dr1 = 2.0;      // depth interval in first section
float dr2 = 5.0;      // depth interval in second section
float dr3 = 20.0;     // depth interval in third section
float per = _PI / 180.0;

float velocity(float, int);
void discont(float, float, float, float, float, float, int);
void tracup(float, float, float, float, int, float *, float *);

int main()
{
    FILE *ins, *in1;
    float vzt, p, ri, r1, r2, v1, v2, v, vm, dd, dt, dr, drp;
    float sina, epi, tout, delt, totime;
    float rzt, r0, zzt, alfa, alfa1, alfa2, da;
    float rz = 6371.0;
    char path[100];
    int ps, ppss, node_id, cont;
    char file_name1[200], file_name2[200];

    dr = 0;

    snprintf(path, 100, "%s/%s", getenv("HOME"),
                                 "DISRAY/config/id");
    in1 = fopen(path, "r");
    fscanf(in1,"%d", &node_id);
    fclose(in1);

    snprintf(path, 100, "%s/%s", getenv("HOME"), 
                                 "DISRAY/config/count");
    in1 = fopen(path, "r");
    fscanf(in1,"%d", &cont);
    fclose(in1);

    snprintf(file_name1, 200,"%s/%s%d", getenv("HOME"), 
                                        "DISRAY/data/map", 
                                        node_id);
    snprintf(file_name2, 200,"%s/%s%d", getenv("HOME"), 
                                        "DISRAY/data/raypath", 
                                        node_id);

    out = fopen(file_name1, "w");
    out2 = fopen(file_name2, "w");

    snprintf(path, 100, "%s/%s", getenv("HOME"), 
                                 "DISRAY/data/source");
    ins = fopen(path,"r");
    fscanf(ins,"%f\n%f", &zzt, &da);

    ps = 1;
    rzt = rz - zzt;
    ri = 6371.0;
    //find source layers
    while(ri >= 0.0) {

        if(rzt <= 6371.0 && rzt > 6336.0)
            dr = dr1;
        else if(rzt <= 6336.0 && rzt > 5961.0)
            dr = dr2;
        else if(rzt <= 5961.0)
            dr = dr3;
        if((rzt-ri) *  (rzt - (ri-dr)) <= 0.0)
            break;

        ri = ri - dr;
    } /*while(ri >= 0.0)*/

    r0 = ri; drp = dr;
    alfa1 = (node_id + 1) * (180.0 / (cont));
    alfa2 = node_id * (180.0 / (cont));

    while(ps <= 2) {
        vzt = velocity(rzt,ps);
        /*printf("\n%f\t%d",vzt,ps);*/

        totime = 0.0;
        epi = 0.0;
        alfa = alfa1;
        while(alfa > alfa2) {

            if(alfa < (90.0 - 0.1)) {

                alfa = alfa * per;
                sina = sin(alfa);
                p = (rzt * sina) / vzt;
                epi = 0.0;
                totime = 0.0;

                ri = r0 - drp;
                fprintf(out2," %.1f,%.2f,%.2f,", rzt, epi, totime);
                r1 = rzt; r2 = ri;
                while(r1 >= 1217.0) {

                    v1 = velocity(r1, ps);
                    v2 = velocity(r2, ps);
                    sina = (r1 * v2 * sina) / (r2 * v1);
                    vm = (v1 + v2) / 2;

                    //reflection from outside of Moho
                    if(r1 > 6336.0 && r2 <= 6336.0) {
                        discont(r1, 6336.0, totime, epi, alfa, p, ps);
                    } /*if(r1> 6336.0 && r2<= 6336.0)*/

                    // reflection from outside of CMB
                    if(r1 > 3483.0 && r2 <= 3483.0)
                        discont(r1, 3483.0, totime, epi, alfa, p, ps);

                    // reflection from ineer-outer core boundery
                    if(r1 > 1217.0 && r2 <= 1217.0)
                        discont(r1, 1217.0, totime, epi, alfa, p, ps);

                    if((v1 * v2) == 0.0)
                        r1 = 0.0;

                    if(sina > 1.0 || sina < -1.0) {

                        if((r2 - 6336.0) * (r1 - 6336.0) <= 0.0 ||
                           (r2 - 3483.0) * (r1 - 3483.0) <= 0.0 ||
                           (r2 - 1217.0) * (r1 - 1217.0) <= 0.0 ||
                           )
                            break;

                        fprintf(out2, ";");
                        tracup(r1, totime, epi, p, ps, &tout, &delt);
                        tout = tout + totime;
                        delt = delt + epi;

                        if(tout < 100000.0)
                            fprintf(out, "%.3f\t%.3f\t%.3f\t%.3f\t%d\n",
                                          delt / per,
                                          alfa / per,
                                          tout,
                                          r1,
                                          ps);

                        break;
                    } /*if(sina>1.0 || sina<-1.0)*/

                    if((p * vm / r1) < -1.0 ||
                       (p * vm / r1) > 1.0  ||
                       (p * vm / r2) < -1.0 ||
                       (p * vm / r2) > 1.0  ||
                       (((r1 * r1) / (vm * vm)) - (p * p)) < 0.0 ||
                       (((r2 * r2) / (vm * vm)) - (p * p)) < 0.0
                      ) {

                        if( (r2 - 6336.0) * (r1 - 6336.0) <= 0.0 ||
                            (r2 - 3483.0) * (r1 - 3483.0) <= 0.0 ||
                            (r2 - 1217.0) * (r1 - 1217.0) <= 0.0 ||
                          )
                            break;

                        fprintf(out2, ";");
                        tracup(r1, totime, epi, p, ps, &tout, &delt);
                        tout = tout + totime;
                        delt = delt + epi;

                        if(delt < 100000.0)
                            fprintf(out, "%.3f\t%.3f\t%.3f\t%.3f\t%d\n",
                                            delt / per,
                                            alfa / per,
                                            tout,
                                            r1,
                                            ps);

                    } /* if((p * vm / r1) <- 1.0 || .... */

                    dd = - (asin(p * vm / r1) - asin(p * vm / r2));

                    dt = sqrt((r1 * r1) / (vm * vm) - (p * p)) 
                       - sqrt((r2 * r2) / (vm * vm) - (p * p));

                    totime = totime + dt;
                    epi = epi + dd;
                    fprintf(out2,"%.1f,%.2f,%.2f,",r2,epi,totime);

                    if(r1 <= 6371.0 && r1 > 6336.0)
                        dr = dr1;
                    else if (r1 <= 6336.0 && r1 > 5961.0)
                        dr = dr2;
                    else if (r1 <= 5961.0)
                        dr = dr3;
                        r1 = r2;
                        r2 = r1 - dr;
                } /*while(r1 >= 1217.0)*/

            /*if(alfa < (90.0 - 0.1))*/
            } else if((alfa - 90.0) > 0.0 || fabs(alfa - 90.0) <= 0.1) {

                epi = 0.0;
                totime = 0.0;
                alfa = alfa * per;
                sina = sin(alfa);
                p = rzt * sina / vzt;
                fprintf(out2, "%.1f,%.2f,%.2f,", rzt, totime, epi);

                r1 = rzt; 
                r2 = r0;
                v1 = vzt; 
                v2 = velocity(r2, ps);
                sina = r1 * v2 * sina / (r2 * v1);
                vm = (v1 + v2) / 2;

                if(sina > 1.0  ||
                   sina < -1.0 ||
                   (p * vm / r1) < -1.0 ||
                   (p * vm / r1) > 1.0  ||
                   (p * vm / r2) < -1.0 ||
                   (p * vm / r2) > 1.0  ||
                   ((r1 * r1) / (vm * vm) - (p * p)) < 0.0 ||
                   ((r2 * r2) / (vm * vm) - (p * p)) < 0.0 ||
                  ) {
                    fprintf(out2,"\n");
                    continue;
                }

                dd = - (asin(p * vm / r2) - asin(p * vm / r1));

                dt = sqrt((r2 * r2) / (vm * vm) - (p * p)) 
                   - sqrt((r1 * r1) / (vm * vm) - (p * p));

                totime = totime + dt;
                epi = epi + dd;

                fprintf(out2,"%.1f,%.2f,%.2f,", r2, epi, totime);

                tracup(r2, totime, epi, p, ps, &tout, &delt);
                totime = totime + tout;
                epi = epi + delt;

                fprintf(out, "%.3f\t%.3f\t%.3f\t%.3f\t%d\n", epi / per,
                                                             alfa / per,
                                                             totime,
                                                             rzt,
                                                             ps);

            } /*else if((alfa - 90.0) > 0.0 || fabs(alfa - 90.0) <= 0.1)*/

            fprintf(out2, "\n");
            alfa = alfa / per;
            alfa = alfa - da;

        } /*while(alfa>alfa2)*/

        ps = ps++;

    } /*while(ps<=2)*/

    fclose(out);
    fclose(out2);
    fclose(ins);

    return 0;

} /*int main()*/



/*
 ********************************************************************
 *                         FUNCTION: discon                         *
 *                                                                  *
 *   This function computes the path of the reflected ray from      *
 *   discontinuties.                                                *
 *   @r1, @r2, @tt: travel time and epicentral distance down to     * 
 *   the reflected point,                                           *
 *   @p: ray parameter,                                             *
 *   @alfa1: incident angle,                                        * 
 *   @ips: P or S ray.                                              *
 *                                                                  *
 ********************************************************************
 */
void discont(float r1, float r2, float tt, float ep, float alfa1, float p, int ips)
{
      float v1, v2, vm, dd, dt, tout1, delt1;
      int ppss;

      v1 = velocity(r1, ips);
      v2 = velocity(r2, ips);
      vm = (v1 + v2) / 2;
      if((p * vm / r1) < -1.0 ||
         (p * vm / r1) > 1.0  ||
         (p * vm / r2) < -1.0 ||
         (p * vm / r2) > 1.0  ||
         ((r1 * r1) / (vm * vm) - (p * p)) < 0.0 ||
         ((r2 * r2) / (vm * vm) - (p * p)) < 0.0 ||
        ) {
         fprintf(out2, ";");
         tracup(r1, tt, ep, p, ips, &tout1, &delt1);
         tout1 = tout1 + tt;
         delt1 = delt1 + ep;
         if(delt1 < 100000.0)
        fprintf(out, "%.3f\t%.3f\t%.3f\t%.3f\t%d\n", delt1 / per,
                                                     alfa1 / per,
                                                     tout1,
                                                     r1,
                                                     ips);

         } else{
        dd = - (asin(p * vm / r1) - asin(p * vm / r2));

        dt = sqrt((r1 * r1) / (vm * vm) - (p * p)) 
           - sqrt((r2 * r2) / (vm * vm) - (p * p));

        tt = tt + dt;
        ep = ep + dd;
        fprintf(out2, "%.1f,%.2f,%.2f", r2, ep, tt);
        tout1 = 0.0;
        if(r2 >= 3483.0) {
            for(ppss = 1; ppss <= 2; ppss++) {
                fprintf(out2,";");
                tracup(r2, tt, ep, p, ppss, &tout1, &delt1);
                tout1 = tout1 + tt;
                delt1 = delt1 + ep;
                if(delt1 >= 100000.0) {
                     fprintf(out2, ",");
                    continue;
                }
                fprintf(out, "%.3f\t%.3f\t%.3f\t%.3f\t%d%d\n", delt1 / per, 
                                                               alfa1 / per,
                                                               tout1,
                                                               r2,
                                                               ips,
                                                               ppss);
            }  /*for(ppss=1;ppss<=2;ppss++)*/
            fprintf(out2, ";");

        /*if(r2 >= 3483.0)*/
        } else if(r2 < 3483.0) {

            fprintf(out2, ";");
            tracup(r2, tt, ep, p, ips, &tout1, &delt1);
            tout1 = tout1 + tt;
            delt1 = delt1 + ep;
            if(delt1 < 100000.0)
                fprintf(out, "%.3f\t%.3f\t%.3f\t%.3f\t%d\n", delt1 / per,
                                                             alfa1 / per,
                                                             tout1,
                                                             r2,
                                                             ips);
                fprintf(out2, ";");
            }
        } /*else if(r2 < 3483.0)*/

} /*void discont(float r1,float ...*/


/*
 ********************************************************************
 *                          FUNCTION: tracup                        *
 *                                                                  *
 *   This function traces ray up to the surface.                    *
 *   @rad: radius,                                                  *
 *   @T and @D: travel time and epicentral distance,                *
 *   @p: ray parameter                                              *
 *   @ips: P or S wave ,                                            *
 *   @time, @epdis: travel time and epicentral distance of up       *
 *   to the surface.                                                *
 *                                                                  *
 ********************************************************************
 */
void tracup(float rad, float T, float D, float p, int ips, float *time, float *epdis)
{
    float ri, r1, r2, v1, v2, v;
    float dd, dt, vm, dr=0;
    *time = 0.0; 
    *epdis = 0.0;
    r1 = rad; 
    r2 = 0;

    while(r2 <= 6371.0) {

        if(r1 <= 6371.0 && r1 > 6336.0)
            dr = dr1;
        else if (r1 <= 6336.0 && r1 > 5961.0)
            dr = dr2;
        else if (r1 <= 5961.0)
            dr = dr3;

        r2 = r1 + dr;
        v1 = velocity(r1, ips);
        v2 = velocity(r2, ips);
        vm = (v1 + v2) / 2;

        if((v1 * v2) == 0.0) {
            *time = 100000.0;
            *epdis = 100000.0;
            break;
        }

        if((p * vm / r1) < -1.0 ||
           (p * vm / r1) > 1.0  ||
           (p * vm / r2) < -1.0 ||
           (p * vm / r2) > 1.0  ||
           ((r1 * r1) / (vm * vm) - (p * p)) < 0.0 ||
           ((r2 * r2) / (vm * vm) - (p * p)) < 0.0 ||
           ) {

            *time = 100000.0;
            *epdis = 100000.0;
            break;
        }

        dt = sqrt((r2 * r2) / (vm * vm) -(p * p)) 
           - sqrt((r1 * r1) / (vm * vm) - (p * p));

        dd = - (asin(p * vm / r2) - asin(p * vm / r1));
        *time = *time + dt;
        *epdis = *epdis + dd;
        T = T + dt;
        D = D + dd;

        fprintf(out2, "%.1f,%.2f,%.2f,", r2, D, T);
        r1 = r2;

    } /*while(r2 <= 6371.0)*/

} /*void tracup(float rad,float */



/*
 *******************************************
 *            FUNCTION:velocity            *
 *                                         *
 *  earth model for spherical ray tracing  *
 *  iasp91 by Kennett and Engdahl (1991).  *
 *                                         *
 *******************************************
 */
float velocity(float rr, int ips)
{
    float rz, x, vmod;

    rz = 6371.0;
    if(rr == 0.0) {
        if(ips == 1) {
            vmod = 11.24094;
        } else if(ips == 2) {
            vmod = 3.56454;
        } /*if(ips==1)*/
    /*if(rr==0.0)*/
    } else if(rr > 0.0 && rr <= 1217.0) {

        /*
         *******************
         *   inner core    *
         *******************
         */
        x = rr / rz;
        if(ips == 1)
            vmod = 11.24094 - 4.09689 * x * x;
        else if(ips == 2)
            vmod = 3.56454 - 3.45241 * x * x;

    } else if (rr > 1217.0 && rr <= 3482.0) {
        /*
         *******************
         *    outer core   *
         *******************
         */
        x = rr / rz;
        if(ips == 1)
            vmod = (10.03904 + 3.75665*x) 
                    - (13.67046 * x * x);
        else if(ips == 2)
            vmod = 0.0;
    } else if(rr > 3482.0 && rr <= 3631.0) {

        x = rr / rz;
        if(ips == 1)
            vmod = 14.49470 - 1.47089 * x;
        else if(ips == 2)
            vmod = 8.16616 - 1.58206 * x;
    } else if (rr > 3631.0 && rr <= 5611.0) {

        x = rr / rz;
        if(ips == 1)
            vmod = 25.1486 - (41.1538 * x) 
                   + (51.9932 * x * x) 
                   - (26.6083 * x * x * x);

        else if(ips == 2)
            vmod = 12.9303 - (21.2590 * x)
                    + (27.8988 * x * x) 
                    - (14.1080 * x * x * x);

    } else if(rr > 5611.0 && rr <= 5711.0) {
        x = rr / rz;
        if(ips == 1)
            vmod = 25.96984 - (16.93412 * x);
        else if(ips == 2)
            vmod = 20.76890 - (16.53147 * x);
    } else if(rr > 5711.0 && rr <= 5961.0) {
        x = rr / rz;
        if(ips == 1)
            vmod = 29.38896 - (21.40656 * x);
        else if(ips == 2)
            vmod = 17.70732 - (13.50652 * x);

    } else if(rr > 5961.0 && rr <= 6161.0) {
        x = rr / rz;
        if(ips == 1)
            vmod = 30.78765 - (23.25415 * x);
        else if(ips == 2)
            vmod = 15.24213 - (11.08552 * x);

    } else if(rr > 6161.0 && rr <= 6251.0) {
        x = rr / rz;
        if(ips == 1)
            vmod = 25.41389 - (17.69722 * x);
        else if(ips == 2)
            vmod = 5.75020 - (1.27420 * x);

    } else if(rr > 6251.0 && rr <= 6336.0) {
        x = rr / rz;
        if(ips == 1)
            vmod = 8.78541 - (0.74953 * x);
        else if(ips == 2)
            vmod = 6.706231 - (2.248585 * x);

    } else if(rr > 6336.0 && rr <= 6351.0) {
        x = rr / rz;
        if(ips == 1)
            vmod = 6.5;
        else if(ips == 2)
            vmod = 3.75;

    } else if(rr > 6351.0 && rr <= 6371.0) {
        x = rr / rz;
        if(ips == 1)
            vmod = 5.8;
        else if(ips == 2)
            vmod = 3.36;
    }

    return vmod;

} /*float velocity(float rr,int ips)*/





