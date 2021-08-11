#include <cstddef>
#include "iniconfopt.h"

#define GET_CONF_VALUE(format) {                     \
    char strValue[128];                              \
    if (!GetValue(section, name, strValue, comment)) \
    {                                                \
        value = defv;                                \
        if (m_bDisplayConfigValue && !m_bItemExist)   \
            SetValue(section, name, defv, NULL);     \
        if (m_bDisplayDefaultPrompt)                 \
        {                                            \
            sprintf(strValue, format, defv);         \
            SetDefaultPrompt(name, strValue);        \
        }                                            \
        return 0;                                    \
    }                                                \
    else                                             \
    {                                                \
        sscanf(strValue, format, &value); \
    } \
}

void Format(const char *string, char *result)
{
    char *p = (char *)string;
    int nFlag = 0;

    if(!IS_STR_VALID(string) || !result)
        return;
    
    while(IS_DELIMITER(*p))
        p++;
    while(*p)
    {
        if(IS_DELIMITER(*p))
        {
            if(nFlag)
            {
                p++;
                continue;
            }
            nFlag = 1;
        }
        else
            nFlag = 0;
        *(result++) = *(p++);
    }
    while(IS_DELIMITER(*(result - 1)))
        result--;

    *result = 0;
}

CSection * CConfOpt::SearchSection(const char *section) {
    CSection *cSection;

    for (cSection = m_dcmSection->Begin(); cSection; cSection = m_dcmSection->Next()) {
        if (!strcmp(cSection->m_strSection, section))
            return cSection;
    }
    return NULL;
}

void CConfOpt::AddSection(const char *section, int column, const char *comment) {
    m_cSection = new CSection(section, column);
    m_dcmSection->Push(m_cSection);
    m_bConfigChange = 1;

    m_chainComment = new CComment("#########################################################");
    m_cSection->SetComment(m_chainComment);
}

void CConfOpt::SetValue(const char *section, const char *name, const int value, const char *comment)
{
    char strTemp[32];
    if(!IS_STR_VALID(section) || !IS_STR_VALID(name))
        return;

    sprintf(strTemp, "%d", value);
    SetValue(section, name, strTemp, comment);
}

void CConfOpt::SetValue(const char *section, const char *name, const char *value, const char *comment)
{
    char strSection[256], strName[128], strValue[128];

    if(!IS_STR_VALID(section) || !IS_STR_VALID(name))
        return;

    Format(section, strSection);
    Format(name, strName);
    if(value)
        strcpy(strValue, value);\
    if(NULL == (m_cSection = SearchSection(strSection)))
    {
        m_cSection = new CSection(strSection, 0);
        m_dcmSection->Push(m_cSection);
    }
}

void CConfOpt::SetDefaultPrompt(const char *name, const char *defv)
{
    if(m_bItemExist)
        return;

    char strComment[128];
    sprintf(strComment, " %s default is \"%s\"", name, defv);
    m_chainComment = new CComment(strComment);
    m_cSection->SetComment(m_chainComment);
}

//得到枚举型配置项目的第N个值
int CConfOpt::GetValue(const char *section, int index, char *value, const char *comment) {
    char strSection[256], *pTemp = NULL;

    if (!IS_STR_VALID(section) || !value)
        return 0;
    Format(section, strSection);
    if (NULL == (m_cSection = SearchSection(strSection))) {
        //配置文件中没有这个标题项，生成这个标题项
        AddSection(strSection, 0, comment);
        return 0;
    }

    pTemp = m_cSection->GetValue(index);
    if (pTemp) {
        strcpy(value, pTemp);
        return 1;
    }else {
        *value = 0;
        return 0;
    }
}

//取整形配置项的值
int CConfOpt::GetValue(const char *section, const char *name, const int defv, int &value, const char *comment)
{
    GET_CONF_VALUE("%d");
    return 1;
}

//取配置项的字符型值
int CConfOpt::GetValue(const char *section, const char *name, char *value, const char *comment)
{
    char strSection[256], strName[128], *pTemp = NULL;
    if(!IS_STR_VALID(section) || !IS_STR_VALID(name) || !value)
        return 0;
    Format(section, strSection);
    Format(name, strName);
    if(NULL == (m_cSection = SearchSection(strSection)))
    {
        //配置文件中没有这个标题项，生成这个标题项
        AddSection(strSection, 0, NULL);
    }
    else
        pTemp = m_cSection->GetValue(strName);

    m_bItemExist = 1;
    if(!pTemp)
    {
        //配置标题中没有这个配置项，增加配置项，并使用缺省值
        m_bConfigChange = 1;
        m_bItemExist = 0;
        if (IS_STR_VALID(comment))
            m_chainComment = new CComment(comment);
        else
            m_chainComment = NULL;
        m_cSection->SetValue(strSection, strName, NULL, m_chainComment);
        return 0;
    }
    else if(!strcmp(pTemp, ""))
    {
        //使用缺省值
        return 0;
    }
    else
    {
        strcpy(value, pTemp);
        return 1;
    }
}

int CGetValueInt(char *szfilename, const char *section, const char *name, int defv, int *value, const char *comment)
{
    CConfOpt Conf(szfilename, 1);
    int ret = Conf.GetValue(section, name, defv, *value, comment);
    return ret;
}