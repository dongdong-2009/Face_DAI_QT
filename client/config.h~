#ifndef CONFIG_H
#define CONFIG_H

#include "global.h"
#include "TSingleIns.h"

class Config:public TSingleIns<Config>
{
public:
    explicit Config() {};
    ~Config(){};
    void ReadConfigData(OptConfig& configData);
private:
    void extract_data(char *line, char *temp);
    void load_file(FILE *s_fp, OptConfig& configData);
    void debug_config_print(OptConfig& configData);
};

#endif//CONFIG_HEAD

