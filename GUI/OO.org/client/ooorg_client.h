#ifndef OOORG_CLIENT_H
#define OOORG_CLIENT_H

#include <exception>

#include <com/sun/star/beans/XPropertySet.hpp>
#include <com/sun/star/bridge/XUnoUrlResolver.hpp>
#include <com/sun/star/container/XIndexAccess.hpp>
#include <com/sun/star/container/XNamed.hpp>
#include <com/sun/star/frame/XComponentLoader.hpp>
#include <com/sun/star/frame/XController.hpp>
#include <com/sun/star/frame/XDesktop.hpp>
#include <com/sun/star/sheet/XCellRangeAddressable.hpp>
#include <com/sun/star/sheet/XSpreadsheetDocument.hpp>
#include <com/sun/star/sheet/XSpreadsheet.hpp>
#include <com/sun/star/sheet/XSpreadsheets.hpp>
#include <com/sun/star/sheet/XSpreadsheetView.hpp>
#include <com/sun/star/table/XCell.hpp>
#include <com/sun/star/table/XCellRange.hpp>
#include <com/sun/star/uno/Sequence.hxx>
#include <com/sun/star/view/PaperOrientation.hpp>
#include <com/sun/star/view/XPrintable.hpp>
#include <cppuhelper/bootstrap.hxx>
#include <osl/file.hxx>
#include <osl/process.h>
#include <rtl/ustring.hxx>

#include <QString>

#include "office_client.h"


typedef ::rtl::OUString OUString;
typedef ::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySet > XPropertySet;
typedef ::com::sun::star::uno::Reference< ::com::sun::star::bridge::XUnoUrlResolver > XUnoUrlResolver;
typedef ::com::sun::star::uno::Reference< ::com::sun::star::container::XIndexAccess > XIndexAccess;
typedef ::com::sun::star::uno::Reference< ::com::sun::star::frame::XComponentLoader > XComponentLoader;
typedef ::com::sun::star::uno::Reference< ::com::sun::star::frame::XController > XController;
typedef ::com::sun::star::uno::Reference< ::com::sun::star::frame::XDesktop > XDesktop;
typedef ::com::sun::star::uno::Reference< ::com::sun::star::frame::XModel > XModel;
typedef ::com::sun::star::uno::Reference< ::com::sun::star::lang::XComponent > XComponent;
typedef ::com::sun::star::uno::Reference< ::com::sun::star::lang::XMultiComponentFactory > XMultiComponentFactory;
typedef ::com::sun::star::uno::Reference< ::com::sun::star::registry::XSimpleRegistry > XSimpleRegistry;
typedef ::com::sun::star::uno::Reference< ::com::sun::star::sheet::XSpreadsheet > XSpreadsheet;
typedef ::com::sun::star::uno::Reference< ::com::sun::star::sheet::XSpreadsheetDocument > XSpreadsheetDocument;
typedef ::com::sun::star::uno::Reference< ::com::sun::star::sheet::XSpreadsheets > XSpreadsheets;
typedef ::com::sun::star::uno::Reference< ::com::sun::star::sheet::XSpreadsheetView > XSpreadsheetView;
typedef ::com::sun::star::uno::Reference< ::com::sun::star::table::XCell > XCell;
typedef ::com::sun::star::uno::Reference< ::com::sun::star::table::XCellRange > XCellRange;
typedef ::com::sun::star::uno::Reference< ::com::sun::star::uno::XComponentContext > XComponentContext;
typedef ::com::sun::star::uno::Reference< ::com::sun::star::uno::XInterface > XInterface;
typedef ::com::sun::star::uno::Sequence< ::com::sun::star::beans::PropertyValue > SequencePropertyValues;
typedef ::com::sun::star::uno::Reference< ::com::sun::star::sheet::XCellRangeAddressable > XCellRangeAddressable;

class Selection;

class OOorgClient : public OfficeClient
{
public:
    OOorgClient();
    ~OOorgClient();
    bool openConnection();
    bool closeConnection();
    bool openDocument();
    
    bool writeToCell(int sheetNum, int row, int column, QString value);
    bool writeToCell(int sheetNum, int row, int column, double value);
    QString readFromCell(int sheetNum, int row, int column);
    bool fillSelection(Selection &s);

private:
    QString convertString(const OUString &src);
    OUString convertString(const QString &src);

    XMultiComponentFactory factory;
    XMultiComponentFactory factoryServer;
    XComponent component;
    XInterface interface;
    XComponentContext context;
    XSpreadsheet spreadsheet;
    XSpreadsheetDocument doc;

};

#endif // OOORG_CLIENT_H
