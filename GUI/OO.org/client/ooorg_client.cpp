#include <QDir>
#include <QMessageBox>

#include <iostream>
#include <cassert>
#include <cstdlib>

#include <com/sun/star/table/CellRangeAddress.hpp>

#include "ooorg_client.h"
#include "selection.h"

const ::com::sun::star::uno::UnoReference_Query UNO_QUERY(::com::sun::star::uno::UNO_QUERY);
using ::com::sun::star::beans::PropertyValue;
using ::com::sun::star::uno::Any;
using ::com::sun::star::table::CellRangeAddress;

static const char* const programRegistry = "fann/fann_plugin.rdb";
static const char* const officeConnectionCommand =
    "uno:socket,host=localhost,port=8100;urp;StarOffice.ServiceManager";

OOorgClient::OOorgClient() :
    factory(NULL),
    factoryServer(NULL),
    component(NULL),
    interface(NULL),
    context(NULL),
    spreadsheet(NULL),
    doc(NULL)
{
    if (openConnection())
    {
        openDocument();
    }
    else
    {
        throw ConnectionFailureException();
    }
    assert(factory != NULL && interface != NULL && "Illegal program state, connection not made");
}

OOorgClient::~OOorgClient()
{
    closeConnection();
}

bool OOorgClient::openConnection()
{
    std::cout << "connecting to OpenOffice..." << std::endl;
    try
    {
        OUString connectionUrl = OUString::createFromAscii(officeConnectionCommand);
        OUString rdbFile = convertString(QString(std::getenv("OFFICE_HOME")) + programRegistry);
        XSimpleRegistry registry(::cppu::createSimpleRegistry());
        registry->open(rdbFile, sal_True, sal_False);

        context = ::cppu::bootstrap_InitialComponentContext(registry);
        this->factory = (context->getServiceManager());

        OUString service = OUString::createFromAscii( "com.sun.star.bridge.UnoUrlResolver" );
        XInterface interface;
        
        interface = factory->createInstanceWithContext(service, context);
        XUnoUrlResolver resolver(interface, UNO_QUERY);
        this->interface = XInterface(resolver->resolve(connectionUrl), UNO_QUERY);

        std::cout << "connection done" << std::endl;
        return true;
    }
    catch (...)
    {
        factory = NULL;
        factoryServer = NULL;
        component = NULL;
        interface = NULL;
        context = NULL;
        spreadsheet = NULL;
        doc = NULL;
        std::cout << "connection failed" << std::endl;
        return false;
    }
}

bool OOorgClient::closeConnection()
{
    return true;
}

bool OOorgClient::openDocument()
{
    try
    {
        std::cout << "opening document..." << std::endl;
        XPropertySet properties(interface, UNO_QUERY);
        OUString name = OUString::createFromAscii("DefaultContext");
        properties->getPropertyValue(name) >>= context;
        
        factoryServer = XMultiComponentFactory(context->getServiceManager());
        OUString service = OUString::createFromAscii("com.sun.star.frame.Desktop");
        interface = factoryServer->createInstanceWithContext(service, context);

        XDesktop desktop(interface, UNO_QUERY);
        component = desktop->getCurrentComponent();
        doc = XSpreadsheetDocument(component, UNO_QUERY);
        XModel model(doc, UNO_QUERY);
        if (model == NULL)
        {
            std::cout << "could not open the document - no active document" << std::endl;
            return false;
        }

        XController controller = model->getCurrentController();
        XSpreadsheetView view(controller, UNO_QUERY);
        if (view == NULL)
        {
            std::cout << "could not open the document - no active spreadsheet" << std::endl;
            return false;
        }
        spreadsheet = view->getActiveSheet();
        if (spreadsheet == NULL)
        {
            std::cout << "could not open the document" << std::endl;
            return false;
        }
        std::cout << "document opened" << std::endl;
        return true;
    }
    catch (...)
    {
        factory = NULL;
        factoryServer = NULL;
        component = NULL;
        interface = NULL;
        context = NULL;
        spreadsheet = NULL;
        doc = NULL;
        std::cout << "could not open the document" << std::endl;
        return false;
    }
}

bool OOorgClient::writeToCell(int sheetNum, int row, int column, double value)
{
    return writeToCell(sheetNum, row, column, QString::number(value));
}

bool OOorgClient::writeToCell(int sheetNum, int row, int column, QString formula)
{
    assert(sheetNum >= 0 && row >= 0 && column >= 0 && "Invalid parameters");
    try
    {
        XSpreadsheets sheets = doc->getSheets();
        XIndexAccess index (sheets, UNO_QUERY);

        Any any = index->getByIndex(sheetNum);

        XSpreadsheet sheet;
        any >>= sheet;

        XCellRange cellRange(sheet, UNO_QUERY);
        XCell cell = cellRange->getCellByPosition(column, row);
        cell->setFormula(convertString(formula));
        return true;
    }
    catch (...)
    {
        return false;
    }

}

bool OOorgClient::fillSelection(Selection &s)
{
    try
    {
        OUString service = OUString::createFromAscii("com.sun.star.frame.Desktop");
        interface = factoryServer->createInstanceWithContext(service, context);
        
        XDesktop desktop(interface, UNO_QUERY);
        component = desktop->getCurrentComponent();
        doc = XSpreadsheetDocument(component, UNO_QUERY);
        XModel model(doc, UNO_QUERY);
        XInterface selection = model->getCurrentSelection();
        XCellRange cellRange(selection, UNO_QUERY);

        CellRangeAddress rangeAddress =
            XCellRangeAddressable(cellRange, UNO_QUERY)->getRangeAddress();
        s.setValid(false);
        s.setSheet(rangeAddress.Sheet);
        s.setStartR(rangeAddress.StartRow);
        s.setStartC(rangeAddress.StartColumn);
        s.setEndR(rangeAddress.EndRow);
        s.setEndC(rangeAddress.EndColumn);
        s.setValid(true);
        return true;
    }
    catch (...)
    {
        s.setValid(false);
        return false;
    }
}

QString OOorgClient::readFromCell(int sheetNum, int row, int column)
{
    assert(sheetNum >= 0 && row >= 0 && column >= 0 && "Invalid parameters");
    try
    {
        XSpreadsheets sheets = doc->getSheets();
        XIndexAccess index (sheets, UNO_QUERY);

        Any any = index->getByIndex(sheetNum);
        XSpreadsheet sheet;
        any >>= sheet;

        XCellRange cellRange(sheet, UNO_QUERY);
        XCell cell = cellRange->getCellByPosition(column, row);
        return QString::number(cell->getValue());
    }
    catch (...)
    {
        return QString();
    }
}

OUString OOorgClient::convertString(const QString &src)
{
    return OUString::createFromAscii(src.toStdString().c_str());
}

QString OOorgClient::convertString(const OUString &src)
{
    QString dst;
    for (int i = 0; i < src.getLength(); ++i)
    {
        dst[i] = src[i];
    }
    return dst;
}
