#include "ai3_bachelor_praktikum.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (argc < 2) {
        QMessageBox::warning(nullptr, "Error", "No arguments have been passed");
        return EXIT_FAILURE;
    }

    std::ifstream fileStream(argv[1]);
    if (!fileStream.is_open()) {
        std::string arg(argv[1]);
        std::string msg = "Could not open file: ";
        msg += arg;
        QMessageBox::warning(nullptr, "Error", msg.c_str());
        return EXIT_FAILURE;
    }

    try {
        std::vector<creaturePrototype> creatureList = creatureFile::parseFile(fileStream);
        if (creatureList.size() == 0) {
            QMessageBox::warning(nullptr, "Error", "No creatures parsed.\nExiting application");
            return EXIT_FAILURE;
        }   
        ai3_bachelor_praktikum w(std::move(creatureList));
        w.show();
        return a.exec();
    } catch (const std::exception& e) {
        QMessageBox::warning(nullptr, "Error", e.what());
        return EXIT_FAILURE;
    }
}
