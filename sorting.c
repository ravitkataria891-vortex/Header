#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "structs.h"
#include "globals.h"
#include "features.h"

soilData soil_array[TABLE_SIZE];
int Soilcount;

cost cost_array[TABLE_SIZE];
int Costcount;

soilData Soildata[200];

cost Costdata[200];

char State[30][20];
int Statecount;

unsigned int hash(char *name)
{
    unsigned int hash_value = 0;
    for (int i = 0; name[i]; i++)
    {
        int c = (int)tolower((unsigned char)name[i]);
        hash_value += c * (i + 1);
        hash_value = (hash_value * c) % TABLE_SIZE;
    }
    return hash_value;
}

void initialise_table_null(void)
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        soil_array[i].occupied = 0;
        cost_array[i].occupied = 0;
    }
}

int hash_lookup_soil(char *city)
{
    unsigned int idx = hash(city);
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        unsigned int pos = (idx + i) % TABLE_SIZE;
        if (!soil_array[pos].occupied)
            return -1;
        if (str_icmp(soil_array[pos].city, city) == 0)
            return (int)pos;
    }
    return -1;
}

int hash_lookup_cost(char *crop)
{
    unsigned int idx = hash(crop);
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        unsigned int pos = (idx + i) % TABLE_SIZE;
        if (!cost_array[pos].occupied)
            return -1;
        if (str_icmp(cost_array[pos].crop, crop) == 0)
            return (int)pos;
    }
    return -1;
}

void LoadSoilData(void)
{
    FILE *fp = fopen("soil_data.csv", "r");
    if (!fp) { printf("  ERROR: Cannot open soil_data.csv!\n"); return; }
    char line[200];
    fgets(line, 200, fp);
    Soilcount = 0;
    while (fgets(line, 200, fp) && Soilcount < 200)
    {
        trim(line);
        if (strlen(line) == 0) continue;
        soilData d;
        d.occupied = 0;
        int n = sscanf(line, "%[^,],%[^,],%[^,],%d,%d,%d,%f,%[^,],%s",
            d.state, d.city,
            d.type, &d.N,
            &d.P, &d.K,
            &d.ph, d.crop, d.season);
        if (n == 9)
        {
            d.occupied = 1;
            Soildata[Soilcount] = d;
            Soilcount++;

            unsigned int idx = hash(d.city);
            for (int j = 0; j < TABLE_SIZE; j++)
            {
                unsigned int pos = (idx + j) % TABLE_SIZE;
                if (!soil_array[pos].occupied)
                {
                    soil_array[pos] = d;
                    break;
                }
            }
        }
    }
    fclose(fp);
}

void LoadCost(void)
{
    FILE *fp = fopen("crop_costs.csv", "r");
    if (!fp) { printf("  ERROR: Cannot open crop_costs.csv!\n"); return; }
    char line[200];
    fgets(line, 200, fp);
    Costcount = 0;
    while (fgets(line, 200, fp) && Costcount < 200)
    {
        trim(line);
        if (strlen(line) == 0) continue;
        cost c;
        c.occupied = 0;
        int m = sscanf(line, "%[^,],%f,%f,%f,%f,%f", c.crop, &c.seedCost,
                         &c.expenses, &c.msp, &c.sellingPrice, &c.yield);
        if (m == 6)
        {
            c.occupied = 1;
            Costdata[Costcount] = c;
            Costcount++;

            unsigned int idx = hash(c.crop);
            for (int j = 0; j < TABLE_SIZE; j++)
            {
                unsigned int pos = (idx + j) % TABLE_SIZE;
                if (!cost_array[pos].occupied)
                {
                    cost_array[pos] = c;
                    break;
                }
            }
        }
    }
    fclose(fp);
}
