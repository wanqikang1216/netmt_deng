#ifndef _ConfigFileOptionClass_H
#define _ConfigFileOptionClass_H

#include <string.h>
#include <stdio.h>

#define __cplusplus 201103L;
#define IS_STR_VALID(string) ((string) && strcmp((string), ""))
#define IS_DELIMITER(c) ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == '\r')

typedef enum _chain_template_equal_method
{
    CtEM_ALLOW = 0,      //允许出现相同节点
    CtEM_REJECT = 77701, //不允许出现相同节点
    CtEM_REPLACE         //替换相同节点
} CtEqualMethod;

//双向不循环链表模板管理类
template <class T, const int sort = 0, const CtEqualMethod equal = CtEM_ALLOW, const int pointer = 1>

class CTDChainMgr
{
//数据成员
public:
}

//配置类结构
class CSection
{
public:
    char m_strSection[256];
    CTDChainMgr<CRecord *, 1, CtEM_REJECT, 1> *m_decmRecoed;
    CTDChainMgr<CEnumValue *, 0, CtEM_REJECT, 1> *m_dcmEnumValue;
    int m_nEnumValueMaxWidth;
    int m_nEnumValueColumn;
    CRecord *m_cRecord;
    CComment *m_chainComment;

public:
    CSection();
    CSection(const char *section, int column);
    ~CSection();

    CEnumValue *SerachEnumValue(int index);
    char *GetValue(int index);
    int SetEnumValue(const char *section, int index, const char *value);
    int SetEnumValue(const char *section, const char *value);
    int EnumValueNum(int column);

    CRecord *SerachRecord(const char *name);
    char *GetValue(const char *name);
    int SetSection(const char *section);
    int SetValue(const char *section, const char *name, const char *value, CComment *&comment);
    void SetComment(const char *comment);
    void SetComment(CComment * &comment);
    int operator == (CSection &right) const;
    int operator==(const char *section) const;
    int operator>(CSection &right) const;
    int operator>=(CSection &) const { return 1; }
    int operator!=(CSection &) const { return 1; }
    int operator<(CSection &) const { return 1; }
    int operator<=(CSection &) const { return 1; }
    void Print(FILE *fp = stdout, bool display_config_value = false);
}

//配置操作类
class CConfOpt
{
private:
    char m_strFileName[256];
    FILE *m_fpFile;
    CTDChainMgr<CSection *, 1, CtEM_REJECT, 1> *m_dcmSection;
    CSection *m_cSection;
    CComment *m_chainComment;

    char *FileName(const char *filename);
    CSection *SearchSection(const char *section);
    void AddSection(const char *section, int column, const char *comment);
    void SetDefaultPrompt(const char *name, const char *defv);

    int m_bConfigChange;
    bool m_bDisplayConfigValue;
    bool m_bDisplayDefaultPrompt;
    int m_bItemExist;

public:
    CConfOpt(int display_config_value = 0, int display_default_prompt = 0);
    CConfOpt(const char *filename, int display_config_value = 0, int display_default_prompt = 0);
    ~CConfOpt();
    void SetWriteFlag(int want_write);
    void FreeMemory();

    //输出配置信息到指定文件
    void WriteToFile(const char *filename = NULL);

    //从指定文件读取配置信息
    void ReadFromFile(const char *filename = NULL);

    //设置指定项目的配置值
    void SetValue(const char *section, const char *name, const int value, const char *comment);
    void SetValue(const char *section, const char *name, const char *value, const char *comment);
    //得到指定项目的配置值
    int GetValue(const char *section, const char *name, const int defv, int &value, const char *comment = NULL);

    //判断配置项是否存在
    int IsExist(const char *section, const char *name);

private:
    int GetValue(const char *section, const char *name, char *value, const char *comment);
};

#ifdef __cplusplus
extern "C"
{
#endif

    int CGetValueInt(char *szfilename, const char *section, const char *name, int defv, int *value, const char *comment = NULL);

#ifdef __cplusplus
}
#endif

#endif