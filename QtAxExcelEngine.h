﻿
/*!
* @copyright  2004-2016  Apache License, Version 2.0 FULLSAIL
* @filename   QtAxExcelEngine.h
* @author     yaoboyuan 254200341@qq.com 原创    Sailzeng <sailerzeng@gmail.com> 改写
* @version
* @date       2016年6月10日
* @brief      这是一个使用 Qt's ActiveX(OLE)，QAxObject 读写EXCEL的封装类，便于操作在使用Qt
*             的代码操作EXCEL，既然是实用OLE,你当然必须先安装EXCEL。
*             
*             这是一个便于Qt读写excel封装的类，同时，便于把excel中的数据
*             显示到界面上，或者把界面上的数据写入excel中，同GUI进行交互，关系如下：
*             Qt tableWidget <--> ExcelEngine <--> xls file.
* @details
*
*
*
* @note       QtAxExcelEngine类只负责读/写数据，不负责解析，做中间层
*
* @history 
* @author     yaoboyuan 254200341@qq.com
* @date       2012-4-12
* @note       原创
* @author     Sailzeng 改写 <sailerzeng@gmail.com>
* @data       2016-6-7 端午节前夕
* @note       对原有的代码做了一些改动，更加符合Qt的编码格式等。
*             增加了一些功能，以及常用函数，更加方便实用
* @author     Sailzeng 改写 <sailerzeng@gmail.com>
* @data       2016-12-16 
* @note       参考 hearstzhang GG 实现，增加了批处理写入的功能，大幅提升了写入速度
*
*/


#ifndef QT_ACTIVEX_EXCELENGINE_H
#define QT_ACTIVEX_EXCELENGINE_H


// 在WINDOWS下和POSIX标准兼容的宏，VS2003以下版本如何，我没有测试，2003以后，Windows对于很多代码宏会使用"_"前缀，
#ifndef _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE 1
#endif
#ifndef _CRT_NONSTDC_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS  1
#endif
#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#endif

// 用于告诉VC++，我不强迫使用_s类型的API，_CRT_SECURE_NO_DEPRECATE是_CRT_SECURE_NO_WARNINGS的老版本
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS   1
#endif
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE  1
#endif

#include <memory>

#include <windows.h>

#pragma warning ( push )
#pragma warning ( disable : 4127)

#include <QtCore/QObject>
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <ActiveQt/QAxBase>
#include <ActiveQt/QAxObject>

#include <QtCore/QDebug>
#include <QtGui/QKeyEvent>
#include <QtCore/QVariant>
#include <QtCore/QDir>

#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTableWidgetItem>

#pragma warning ( pop )

/*!
* @brief
*
* note       代码风格我打算全部采用Qt的代码风格。
*/
class QtAxExcelEngine : protected QObject
{
public:

    //构造函数和析构函数
    QtAxExcelEngine();
    ~QtAxExcelEngine();

public:


    /*!
    * @brief      初始化EXCEL OLE对象，打开EXCEL 进程，
    * @return     bool  返回是否成功打开
    * @param      visible 是否让EXCEL可见
    */
    bool initialize(bool visible);

    //!释放退出，关闭EXCEL进程
    void finalize();

	/*!
	* @brief      打开一个XLS文件,文件不存在或者打开失败返回false,不会新建
	* @return     bool          true表示打开成功
	* @param      xls_file      打开的文件名称,
	* @param      not_exist_new 如果要打开的文件不存在，是否新建
	*/
	bool open(const QString &xls_file,
			  bool not_exist_new);
	
	//!新建一个XLS文件
	bool newOne();

	//保存xls报表
	void save();
	//!
	void saveAs(const QString &xls_file);

    //关闭xls报表
    void close();

    //得到sheet的总数
    int sheetsCount();

    //得到某个sheet的名字
    bool sheetName(int sheet_index, QString &sheet_name);

    /*!
    * @brief      根据索引加载sheet，
    * @return     bool 返回是否成功加载
    * @param      sheet_index sheet索引，从1开始
	* @param      pre_read 是否进行预加载，预加载会读取整个EXCEL的sheet表，加快后面的读取处理
    */
    bool loadSheet(int sheet_index,
				   bool pre_read = false);

    /*!
    * @brief      根据sheet表格表名加载sheet，
    * @return     bool 返回是否成功加载
    * @param      sheet_name 要加载的sheet 的名字
	* @param      pre_load 是否进行预加载，预加载会读取整个EXCEL的sheet表，加快后面的读取处理，预加载最好是只读时使用
    */
    bool loadSheet(const QString &sheet_name,
				   bool pre_read = false);


    /*!
    * @brief      检查是否有这个名字的sheet
    * @return     bool 有返回true，否则返回false
    * @param      sheet_name
    */
    bool hasSheet(const QString &sheet_name);


	/*!
	* @brief      插入一个sheet，同时加载这个表格，
	* @param      sheet_name sheet的表名
	*/
	void insertSheet(const QString &sheet_name);

	
	/*!
	* @brief      对当前的Sheet进行改名操作，
	* @param      new_name 新的名字
	*/
	void renameSheet(const QString &new_name);


    /*!
    * @brief      把table_widget中的数据保存到excel中，
    * @return     bool
    * @param      table_widget
    */
    bool writeTableWidget(QTableWidget *table_widget);

    //从xls读取数据到ui
    bool readTableWidget(QTableWidget *table_widget);

    /*!
    * @brief      获取指定单元数据
    * @return     QVariant  单元格对应的数据
    * @param      row  单元格的行号,注意行号，列号都是从1开始
    * @param      column 单元格的列号
    * @note
    */
    QVariant getCell(int row, int column);
    
    /*!
    * @brief      修改指定单元数据
    * @return     bool  是否修改成功
    * @param      row    行号
    * @param      column 列号
    * @param      data   修改的数据
    */
    bool  setCell(int row, int column, const QVariant &data);

    /*!
    * @brief      从一个RANGE中读取sheet的数据，
    * @param      cell1_row     range的起始点，左上的行号
    * @param      cell1_column  range的起始点，左上的列号
    * @param      cell2_row     range的结束点，右下的行号
    * @param      cell2_column  range的结束点，右下的列号
    * @param      data_table  要写入的数据，data_table是一个table，也就是QVariantList(行)套QVariantList(列)
    */
	void getRangecell(int cell1_row,
					  int cell1_column,
					  int cell2_row,
					  int cell2_column, 
					  QVariantList &data_table);

	//向一个RANGE中写入sheet的数据，QVariantList是一个table，也就是List套List
	bool setRangeCell(int cell1_row,
					  int cell1_column,
					  int cell2_row,
					  int cell2_column,
					  const QVariantList &data_table);

    bool setColumnCell(int rowNo , int TargetColumnNo , int rowCount ,QVariant dataList);


    //!打开的xls文件名称
    QString openFilename() const;

    //!当前Sheet的行数,包括空行 
    int  rowCount() const;
    //!当前Sheet的列数,包括空行 
    int  columnCount() const;

	//!当前的Sheet的起始行数，如果第1,2行是空，没有数据，那么返回3
	int startRow() const;
	//!当前的Sheet的起始列数，如果第1,2,3列是空，没有数据，那么返回4
	int startColumn() const;

	//!是否打开了一个EXCEL
    bool isOpen();
	
	//!当前book 是否保存了。
	bool isSaved();

protected:

    //!加载，内部函数，以后可以考虑增加一个预加载，加快读取速度。
    void loadSheet_internal(bool pre_load);

	//!打开（新建）文件的内部具体实现
	bool opennew_internal(bool new_file);

public:

    /*!
    * @brief      取得列的名称，比如27->AA
    * @return     QString
    * @param      column_no
    */
    static QString columnName(int column_no);


    /*!
    * @brief      取得Cell的名称，比如第1行1列cell是A1,第5行5列是E5，注意输出的是列号在前面
    * @return     QString
    * @param      row_no
    * @param      column_no
    */
    static QString cellsName(int row_no, int column_no);

private:

	//!COMM是否是自己初始化的，因为多个实例等情况下，可能是人家初始化的，这时候退出是不主动释放
	bool com_init_byself_ = false;

    //!指向整个excel应用程序
    QAxObject *excel_instance_ = NULL;

    //!指向工作簿集,excel有很多工作簿，你可以简单理解EXCEL会打开很多个文件
    QAxObject *work_books_ = NULL;


    //!指向sXlsFile对应的工作簿
    QAxObject *active_book_ = NULL;

	//!
	QAxObject *work_sheets_ = NULL;

    //指向工作簿中的某个sheet表单
    QAxObject *active_sheet_ = NULL;

    //!xls文件路径
    QString xls_file_;

	//预加载的表的数据
	QVariantList preload_sheet_data_;

    //!excel是否可见
    bool is_visible_ = false;

    //!行数,包括起始的空行
    int row_count_ = 0;
    //!列数,包括起始的空列
    int column_count_ = 0;

    //!开始有数据的行下标值
    int start_row_ = 1;
    //!开始有数据的列下标值
    int start_column_ = 1;

    //!是否已打开
    bool is_open_ = false;

	//是否是一个新建的文件
	bool is_newfile_ = false;

};

#endif // QT_EXCELENGINE_H
