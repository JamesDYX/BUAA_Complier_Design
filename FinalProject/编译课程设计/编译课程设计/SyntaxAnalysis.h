﻿/*
** @author:止水清潇menghuanlater
** @date:2017-11-22
** @location:BUAA
*/
#ifndef SYNTAXANALYSIS_H
#define SYNTAXANALYSIS_H

#include "LexicalAnalysis.h"
#include "error.h"
#include "ConstValue.h"
#include "SymbolTable.h"
using namespace std;

extern vector<SymbolTableItem> globalSymbolTable; //使用全局的符号表

class SyntaxAnalysis
{
private:
    Error & myError;//公用错误处理
    LexicalAnalysis & myLexicalAnalysis;//公用词法分析
    bool isMainVoid; //main函数分析的前缀是否是void
	///相关递归下降子程序需要返回的值
	int return_integer;//ZSQX_integer()需要返回的识别的整数值
	string return_declare_funcName;
	///
    int getLineNumber(){//获取词法分析当前行
        return myLexicalAnalysis.getLineCount();
    }
	//下面是填符号表,查符号表语义分析相关的操作函数
	//填符号表函数,采用函数重载
	void pushItem(string id, string functionName, int num);//常量 整数
	void pushItem(string id, string functionName, char character);//常量 char
	void pushItem(string id, string functionName, ValueType valueType, int size);//变量 数组
	void pushItem(string id, string functionName, FunctionType funcType);//函数
	void pushItem(string id, string functionName, ItemType itemType, ValueType valueType);//变量与参数
	//检查是否可以填表
	bool isAbleInsert(string id, string functionName);
	//引用变量函数等需要检查是否定义
	bool isDefined(string id, string functionName);
	//

public:
    //标准构造函数
    SyntaxAnalysis(Error & myError,LexicalAnalysis & myAnalysis);
    //程序入口函数
    void startAnalysis();
    //下面是所有的递归下降子程序法定义的分析过程
    //＜程序＞ ::= ［＜常量说明＞］［＜变量说明＞］[<函数定义部分>]   ＜主函数＞
    bool ZSQX_procedure();
    //＜常量说明＞ ::= const＜常量定义＞;{ const＜常量定义＞;}
    bool ZSQX_constDescription(string funcName);
    /*＜常量定义＞ ::= int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞}
        | char＜标识符＞＝＜字符＞{,＜标识符＞＝＜字符＞} */
    bool ZSQX_constDefinition(string funcName);
    //＜变量说明＞ ::= ＜变量定义＞;{＜变量定义＞;}
    bool ZSQX_varDescription(bool isGlobal, string funcName);
    /*＜变量定义＞ ::= ＜类型标识符＞(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’)
            {,(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’ )}*/
    bool ZSQX_varDefinition(string funcName);
    //<函数定义部分> ::= {＜有返回值函数定义＞|＜无返回值函数定义＞}
    bool ZSQX_functionDefinition();
    /*＜有返回值函数定义＞ ::= ＜声明头部＞‘(’＜参数表＞‘)’ ‘{’＜复合语句＞‘}’|＜声明头部＞‘{’＜复合语句＞‘}’*/
    bool ZSQX_haveReturnValueFunctionDefinition();
    //＜无返回值函数定义＞ ::= bool＜标识符＞(’＜参数表＞‘)’‘{’＜复合语句＞‘}’| bool＜标识符＞{’＜复合语句＞‘}’
    bool ZSQX_noReturnValueFunctionDefinition();
    //＜参数表＞ ::= ＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}
    bool ZSQX_paramTable(string funcName);
    //＜复合语句＞ ::= ［＜常量说明＞］［＜变量说明＞］｛＜语句＞｝
    bool ZSQX_compoundStatement(string funcName);
    //＜表达式＞ ::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}
    bool ZSQX_expression();
    //＜项＞ ::= ＜因子＞{＜乘法运算符＞＜因子＞}
    bool ZSQX_item();
    //＜因子＞ ::= ＜标识符＞[‘(’<值参数表>‘)’]｜＜标识符＞‘[’＜表达式＞‘]’|‘(’＜表达式＞‘)’｜＜整数＞|＜字符＞
    bool ZSQX_factor();
    /*＜语句＞ ::= ＜条件语句＞｜＜循环语句＞| ‘{’｛＜语句＞｝‘}’｜＜标识符＞[‘(’<值参数表>‘)’]; 
        ｜＜赋值语句＞;｜＜读语句＞;｜＜写语句＞;｜＜空＞;|＜情况语句＞｜＜返回语句＞;*/
    bool ZSQX_statement();
    //＜赋值语句＞ ::= ＜标识符＞＝＜表达式＞|＜标识符＞‘[’＜表达式＞‘]’=＜表达式＞
    bool ZSQX_assignStatement();
    //＜条件语句＞::= if ‘(’＜条件＞‘)’＜语句＞else＜语句＞
    bool ZSQX_conditionStatement();
    //＜条件＞ ::= ＜表达式＞＜关系运算符＞＜表达式＞｜＜表达式＞ 
    bool ZSQX_condition();
    //＜循环语句＞ ::= while ‘(’＜条件＞‘)’＜语句＞
    bool ZSQX_loopStatement();
    //＜情况语句＞ ::= switch ‘(’＜表达式＞‘)’ ‘{’＜情况表＞[＜缺省＞] ‘}’
    bool ZSQX_situationStatement();
    //＜情况表＞ ::= ＜情况子语句＞{＜情况子语句＞}
    bool ZSQX_situationTable();
    //＜情况子语句＞ ::= case＜常量＞：＜语句＞
    bool ZSQX_situationSonStatement();
    //＜缺省＞ ::= default : ＜语句＞
    bool ZSQX_default();
    //＜值参数表＞ ::= ＜表达式＞{,＜表达式＞}
    bool ZSQX_valueParamTable();
    //＜读语句＞ ::= scanf ‘(’＜标识符＞{,＜标识符＞}‘)’
    bool ZSQX_readStatement();
    //＜写语句＞ ::= printf ‘(’ ＜字符串＞,＜表达式＞ ‘)’| printf ‘(’＜字符串＞ ‘)’| printf ‘(’＜表达式＞‘)’
    bool ZSQX_writeStatement();
    //＜返回语句＞ ::= return[‘(’＜表达式＞‘)’]
    bool ZSQX_returnStatement();
    //＜整数＞ ::= ［＋｜－］＜无符号整数＞｜0
    bool ZSQX_integer();
    //＜声明头部＞ ::= int ＜标识符＞ |char ＜标识符＞
    bool ZSQX_declareHead();

};

#endif