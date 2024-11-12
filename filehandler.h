#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QObject>
#include <QString>
#include <QFileDialog>
#include <QStandardPaths>

/**
 * @brief The FileHandler class handles file and folder operations for encryption/decryption
 *
 * This class provides functionality to select, process, and manage files and folders
 * for both encryption and decryption operations. It maintains paths and states
 * for files/folders being processed and handles password management.
 */

class FileHandler : public QObject
{
    Q_OBJECT
    // Qt properties for QML integration
    Q_PROPERTY(QString filePath READ filePath WRITE setFilePath NOTIFY filePathChanged)
    Q_PROPERTY(QString folderPath READ folderPath WRITE setFolderPath NOTIFY folderPathChanged)
    Q_PROPERTY(QString folderPathDecrypt READ folderPathDecrypt WRITE setFolderPathDecrypt NOTIFY folderPathChangedDecrypt)
    Q_PROPERTY(bool isFolder READ isFolder NOTIFY isFolderChanged)
    Q_PROPERTY(bool isFolderDecrypt READ isFolderDecrypt NOTIFY isFolderChangedDecrypt)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)
    Q_PROPERTY(QString fileSize READ fileSize WRITE setFileSize NOTIFY fileSizeChanged )

public:
    /** @brief Constructs a FileHandler object */
    explicit FileHandler(QObject *parent = nullptr);

    QString fileName()const{return m_fileName;}
    /** @brief Gets the current encryption/decryption password */
    QString password() const { return m_password; }

    /** @brief Gets the path of the file to be encrypted */
    QString filePath() const { return m_filePath; }

    /** @brief Gets the path of the folder to be encrypted */
    QString folderPath() const { return m_folderPath; }


    /** @brief Gets the path of the folder to be decrypted */
    QString folderPathDecrypt() const { return m_folderPathDecrypt; }

    /** @brief Checks if the current operation is on a folder */
    bool isFolder() const { return m_isFolder; }

    /** @brief Checks if the current decryption operation is on a folder */
    bool isFolderDecrypt() const { return m_isFolderDecrypt; }

    /** @brief Sets the path for file encryption */
    void setFilePath(const QString &path);



    /** @brief Sets the path for folder encryption */
    void setFolderPath(const QString &path);

    /** @brief Sets the path for folder decryption */
    void setFolderPathDecrypt(const QString &path);
/** @brief Sets the path for folder decryption */

    // Path storage variables
    QString pathToSave;           ///< Path where encrypted files will be saved
    QString pathToSaveDecrypt;    ///< Path where decrypted files will be saved
    QString myPassword;           ///< Current encryption/decryption password
    QString m_fileSize;           ///< Current File size
    QString myFile;
    /** @brief Gets the size of a file at the specified path */
    QString fileSize()const{return m_fileSize;}
public slots:
    /** @brief Opens a file dialog for selecting a file to encrypt */
    void selectFile();

    /** @brief Opens a folder dialog for selecting a folder to encrypt */
    void selectFolder();


    /** @brief Opens a folder dialog for selecting a folder to decrypt */
    void selectFolderDecrypt();

    /** @brief Processes the selected file for encryption */
    void processFile(const QString &path);

    /** @brief Processes the selected folder for encryption */
    void processFolder(const QString &path);

    /** @brief Processes the selected folder for decryption */
    void processFolderDecrypt(const QString &path);

    /** @brief Validates and sets the encryption/decryption password */
    void selectPassword(const QString &password);

    /** @brief Validates and sets the encryption/decryption password */
    void selecFileName(const QString &fileName);

    /** @brief Sets the encryption/decryption password */
    void setPassword(const QString &password);

    void setFileSize(const QString &size);

    /** @brief Sets the file name */
    void setFileName(const QString &fileName);
signals:
    // Signal declarations for property changes and operation results
    void filePathChanged();
    void folderPathChanged();
    void folderPathChangedDecrypt();
    void isFolderChanged();
    void isFolderChangedDecrypt();
    void fileProcessed(bool success);
    void folderProcessed(bool success);
    void folderProcessedDecrypt(bool success);
    void passwordSelected(bool success);
    void passwordChanged();
    void fileSizeChanged();
    void fileNameChanged();
    void fileNameSelected(bool success);
private:
    // Private member variables
    QString m_password;
    QString m_filePath;
    QString m_folderPath;
    QString m_folderPathDecrypt;
    bool m_isFolder;
    bool m_isFolderDecrypt;
    QString m_fileName;

};

#endif // FILEHANDLER_H
