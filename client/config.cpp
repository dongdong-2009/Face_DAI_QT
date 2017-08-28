#include "config.h"

void Config::extract_data(char *line, char *temp)
{
	int i = 0, j = 0;

	for (; line[i] != '='; i++)
		;
	i++;
	for (; i < strlen(line); i++)
	{
		if (line[i] == ' ')
			continue;
		temp[j++] = line[i];
	}
	temp[j] = '\0';
}

void Config::load_file(FILE *s_fp, OptConfig& configData)
{
    char line[MAXLINE] ={0};
    char temp[MAXDATA] ={0};
    int len  = 0;
    while (fgets(line, MAXLINE, s_fp) != NULL)
    {
	    if (strstr(line, "Ip") != NULL)
	    {
	        extract_data(line, temp);
	        len = strlen(temp);
		    strncpy(configData.serverIp, temp, len); 
	    }

	    if (strstr(line, "Port") != NULL)
	    {
		    extract_data(line, temp);
            configData.serverPort = atoi(temp);
	    }
    }
}

void Config::debug_config_print(OptConfig& configData)
{
	PRT_INFO("server Ip : %s", configData.serverIp);
	PRT_INFO("server Port : %d\n", configData.serverPort);
}

void Config::ReadConfigData(OptConfig& configData)
{
	FILE *s_fp;

	if ((s_fp = fopen("./config.ini", "r+")) == NULL)
	{
		printf("[-] open option.ini failed.\n");
	}

	load_file(s_fp, configData);
	fclose(s_fp);

	debug_config_print(configData);
}
