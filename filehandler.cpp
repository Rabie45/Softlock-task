// filehandler.cpp
#include "filehandler.h"
#include <QDebug>
#include <QDir>
#include "AES.cpp"
#include <QMessageBox>
#include <iostream>
#include <fstream>
FileHandler::FileHandler(QObject *parent)
    : QObject(parent)
    , m_filePath("")
    , m_folderPath("")
    , m_folderPathDecrypt("")
    , m_isFolder(false)
    , m_isFolderDecrypt(false)
    , m_password("")
    , m_fileName("decrypted")
{
}

void FileHandler::setFolderPathDecrypt(const QString &path)
{
    if (m_folderPathDecrypt != path) {
        m_folderPathDecrypt = path;
        m_isFolderDecrypt = true;
        emit folderPathChangedDecrypt();
        emit isFolderChangedDecrypt();
        processFolderDecrypt(path);
    }
}

void FileHandler::setFilePath(const QString &path)
{
    if (m_filePath != path) {
        m_filePath = path;
        m_isFolder = false;
        emit filePathChanged();
        emit isFolderChanged();
        processFile(path);
    }
}

void FileHandler::setFolderPath(const QString &path)
{
    if (m_folderPath != path) {
        m_folderPath = path;
        m_isFolder = true;
        emit folderPathChanged();
        emit isFolderChanged();
        processFolder(path);
    }
}

void FileHandler::selectFile()
{
    if (m_password.isEmpty()) {
        qDebug() << "Error: Please select a password first";
        QMessageBox::critical(nullptr, "Error",
                              QString("Please select a password first"),
                              QMessageBox::Ok);
        return;
    }

    QString fileName = QFileDialog::getOpenFileName(
        nullptr,
        tr("Select File"),
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
        tr("All Files (*.*)")
        );

    if (!fileName.isEmpty()) {
        setFilePath(fileName);
    }
}

void FileHandler::selectFolder()
{
    if (m_password.isEmpty()) {
        qDebug() << "Error: Please select a password first";
        QMessageBox::critical(nullptr, "Error",
                              QString("Please select a password first"),
                              QMessageBox::Ok);
        return;
    }
    QString folderName = QFileDialog::getExistingDirectory(
        nullptr,
        tr("Select Folder"),
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        );

    if (!folderName.isEmpty()) {
        setFolderPath(folderName);
    }
}
void FileHandler::processFolderDecrypt(const QString &path)
{
    if (path.isEmpty()) {
        emit folderProcessedDecrypt(false);
        return;
    }

    QDir dir(path);
    if (!dir.exists()) {
        qDebug() << "Invalid folder path:" << path;
        QMessageBox::critical(nullptr, "Error",
                              "Invalid file path: " + path + "\nPlease select a valid file.",
                              QMessageBox::Ok);
        emit folderProcessedDecrypt(false);
        return;
    }

    qDebug() << "Processing folder for decryption:" << path;
    pathToSaveDecrypt = path;

    emit folderProcessedDecrypt(true);
}
void FileHandler::selectFolderDecrypt()
{

    if (m_password.isEmpty()) {
        qDebug() << "Error: Please select a password first";
        QMessageBox::critical(nullptr, "Error",
                              QString("Please select a password first"),
                              QMessageBox::Ok);
        return;
    }

    QString folderName = QFileDialog::getExistingDirectory(
        nullptr,
        tr("Select Folder to Decrypt"),
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        );

    if (!folderName.isEmpty()) {
        setFolderPathDecrypt(folderName);
    }
}
void FileHandler::processFile(const QString &path)
{
    if (path.isEmpty()) {
        emit fileProcessed(false);
        return;
    }

    QFileInfo fileInfo(path);
    if (!fileInfo.exists() || !fileInfo.isFile()) {
        QMessageBox::critical(nullptr, "Error",
                              "Invalid file path: " + path + "\nPlease select a valid file.",
                              QMessageBox::Ok);
        emit fileProcessed(false);

        return;
    }

    emit passwordChanged();
    emit fileNameChanged();
    qDebug() << "Processing file:" << path;
    qDebug() << "Path To Save:" << pathToSave;
    qDebug() <<"Path to retreive: "<<pathToSaveDecrypt;
    qDebug() << "Password:" << myPassword;
    qDebug() << "myFile:" << myFile;
    qint64 fileSize = fileInfo.size();  // Returns the file size in bytes[1]
    QString fileName = "File name: "+fileInfo.fileName()+ " Size:"+ QString::number(fileSize)+" Byte";  // Returns complete file name with extecntion
    setFileSize(fileName);
    qDebug() << "File Info:" << fileName;
    try {
    std::string password =myPassword.toStdString() ;

    // Read input file
    AESCipher cipher(password);

    // First check if input file exists
    std::ifstream check_file(path.toStdString());
    if (!check_file.good()) {
        throw std::runtime_error("Input file 'plaintext.txt' not found");
    }
    check_file.close();

    // Read input file
    std::ifstream input_file(path.toStdString(), std::ios::binary);
    std::vector<unsigned char> plaintext(
        (std::istreambuf_iterator<char>(input_file)),
        std::istreambuf_iterator<char>()
        );
    input_file.close();

    if (plaintext.empty()) {
        throw std::runtime_error("Input file is empty");
    }

    // Encrypt
    std::vector<unsigned char> ciphertext = cipher.encrypt(plaintext);
    pathToSave+="/"+ fileInfo.baseName()+".enc";
    // Save encrypted data
    std::ofstream encrypted_file(pathToSave.toStdString(), std::ios::binary);
    if (!encrypted_file) {
        throw std::runtime_error("Could not create encrypted.bin");
    }
    encrypted_file.write(reinterpret_cast<const char*>(ciphertext.data()),
                         ciphertext.size());
    encrypted_file.close();

    // Verify encrypted file exists before reading
    std::ifstream verify_encrypted(pathToSave.toStdString());
    if (!verify_encrypted.good()) {
        throw std::runtime_error("Encrypted file was not created successfully");
    }
    verify_encrypted.close();

    // Read the encrypted file
    std::ifstream encrypted_input(pathToSave.toStdString(), std::ios::binary);
    std::vector<unsigned char> encrypted_data(
        (std::istreambuf_iterator<char>(encrypted_input)),
        std::istreambuf_iterator<char>()
        );
    encrypted_input.close();

    // Decrypt the data
    std::vector<unsigned char> decrypted = cipher.decrypt(encrypted_data);
    pathToSaveDecrypt+="/"+myFile+".txt";
    // Save decrypted data
    std::ofstream decrypted_file(pathToSaveDecrypt.toStdString(), std::ios::binary);
    if (!decrypted_file) {
        throw std::runtime_error("Could not create decrypted.txt");
    }
    decrypted_file.write(reinterpret_cast<const char*>(decrypted.data()),
                         decrypted.size());
    decrypted_file.close();

    std::cout << "Encryption and decryption completed successfully!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    // Add your file processing logic here
    emit fileProcessed(true);
}

void FileHandler::processFolder(const QString &path)
{
    if (path.isEmpty()) {
        emit folderProcessed(false);
        return;
    }

    QDir dir(path);
    if (!dir.exists()) {
        qDebug() << "Invalid folder path:" << path;
        emit folderProcessed(false);
        return;
    }

    qDebug() << "Processing folder:" << path;
    pathToSave = path;

    QStringList files = dir.entryList(QDir::Files);
    for (const QString &file : files) {
        qDebug() << "Found file:" << file;
    }
    emit folderProcessedDecrypt(true);
    emit folderProcessed(true);
}
void FileHandler::setFileName(const QString &fileName){
    if(m_fileName!=fileName){
        m_fileName=fileName;
        myFile=m_fileName;
        qDebug() << "FileName :" << m_fileName;
        emit fileNameChanged();
    }
}
void FileHandler::setPassword(const QString &password)
{
    if (m_password != password) {
        m_password = password;
        myPassword = m_password;
        qDebug() << "setPassword:" << myPassword;
        emit passwordChanged();
    }
}
void FileHandler::selecFileName(const QString &fileName){
    if (fileName.isEmpty()) {
        qDebug() << "Error: File name empty";
        emit fileNameSelected(false);
        return;
    }
    m_fileName=fileName;
    myFile = m_fileName;
    qDebug() << "myFileooo:" << myFile;
    emit fileNameSelected(true);
    emit fileNameChanged();
}
void FileHandler::selectPassword(const QString &password)
{
    if (password.isEmpty()) {
        qDebug() << "Error: Password is empty";
        emit passwordSelected(false);
        return;
    }

    m_password = password;
    myPassword = m_password;
    qDebug() << "Password selected:" << myPassword;
    emit passwordSelected(true);
    emit passwordChanged();
}

void FileHandler::setFileSize(const QString &size){
    qDebug()<<__FUNCTION__<<"size: "<<size;
    if(m_fileSize !=size){
        m_fileSize=size;
        emit fileSizeChanged();
    }
}
