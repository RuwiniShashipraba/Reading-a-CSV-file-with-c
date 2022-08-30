#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * return the position of the key within the csv header
 * return -1 if the key is not found
 * @csv_header header of the csv file
 * @key the name of the field you are looking for
 */
int getFieldIndex(char *csv_head, char *key);

/**
 * return the value of the ith field of the line
 * works only with integer fields
 * @line the line from which to extract the value
 * @index of the field
 * Can't fail unless the CSV file is miss formed or index is erroneous
 */
int getFieldValue(char *line, int field);

/**
 * return the value of the ith field of the line
 * works only with strings
 * @line the line from which to extract the value
 * @index of the field
 * @results where results are to be placed
 * return 1 if the field is found 0 otherwise
 * Can't fail unless the CSV file is miss formed
 */
void getFieldString(char *line, int filed, char *results);


/**
 * The main function
 * User is expected to give two fileds country and field (that is cases or
 * deaths). We will assume the given country is always there
 */
 
 #define COUNTRY "countriesAndTerritories"
// in the CSV header this is what the country is called

#define DATE "dateRep"

#define DATAFILE "covid-19.csv"
// where the data is kept

int main(int argc, char **argv)
{
  if(argc != 3) { // wrong number of arguments
    printf("You need to give two arguments\n");
    return -1;
  }

  char *country = argv[1];
  char *field   = argv[2];

  printf("Analysing %s's data for the maximum %s\n",
	 country, field);

  char line[300];

  // lets open the file
  FILE *fp = fopen(DATAFILE,"r");
  if(fp == NULL) {
    printf("Can't open %s\nDid you download it???",DATAFILE);
    return -1;
  }

  if(fgets(line, 300, fp) == NULL) { //read about fgets
    // fgets reads 300 chars or untill it finds a newline or EOF
    printf("Can read the data file\n");
    return -1;
  }

  int countryIndex = getFieldIndex(line, COUNTRY);
  int fieldIndex   = getFieldIndex(line, field);
  int dateIndex    = getFieldIndex(line, DATE);

  if(countryIndex == -1 ||
     fieldIndex   == -1 ||
     dateIndex    == -1) {
    printf("Can't find the fileds in header\n");
    return -1;
  }

  // lets read one line at a time.
  // see if the line corresponds to the country
  // if so extract the filed (used wants)
  int previous = 0, current = 0, max = 0;
  char maxDate[11];
  char countryName[40];

  while(fgets(line, 300, fp) != NULL) {

      getFieldString(line, countryIndex, countryName); //extract country name

      // if extracted name is what used asked
      if(strcmp(countryName, country) == 0) {
	current = getFieldValue(line, fieldIndex);
	if(previous - current > max) {
	  max = previous - current;
	  getFieldString(line, dateIndex, maxDate);
	}
	previous = current;
      }
    }
  printf("In %s max %s was %d and was reported on %s\n",
	 country, field, max, maxDate);

  return 0;
}


int getFieldIndex(char *head, char *key)
{//Identify the field index
  int i,j=0,count=0;
  for(i=0; head[i] != '\0'; i++){
	    if(head[i] == ','){
		count++;  
	  }
	    if(head[i]==key[j]&& key[j] != '\0'){
		    j++;
			if(j == strlen(key)){
				return count;
				break;
			}
		}
	    else
			j=0;
		
  }
  return -1;
  
}

int getFieldValue(char *line, int filed)
{
  int i, j = 0 ,count = 0,value;
  char ch[10];

	for(i=0 ; line[i] != '\0'; i++){
		if(line[i] == ','){
			count++;
	    }
		if(count == filed){
			if(line[i] == ','){
			continue;
			}
			ch[j] = line[i];
			j++;

		}
		if(count > filed){
			int value = atoi(ch);//convert string to integer
			return value;
			break;
		}

	}
	return -1;
}

void getFieldString(char *line, int field, char *result)
{
  int i,j = 0 , count = 0;
    
	for(i=0; line[i] != '\0'; i++){
		if(line[i] == ','){
			count++;
		}
		if(count == field ){
			if(line[i] == ','){
			continue;
			}
			result[j] = line[i];
			j++;

		}
		if(count > field){
			break;
		}
	}
	result[j] = '\0';
	return ;
}
