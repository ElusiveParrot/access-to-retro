#include <QMessageBox>
#include <QFile>
#include "qt-helper.hpp"

void ar::helpers::qt::wait_with_event_processing(ar::util::time_duration duration)
{
    QTime end_time = QTime::currentTime().addSecs(static_cast<int>(duration.get_total_seconds()));
    while (QTime::currentTime() < end_time)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

void ar::helpers::qt::execute_splash_screen_event(const QApplication& app, QSplashScreen& splash,
                                                  const std::function<void()>& action, const QString& waiting_message)
{
    splash.showMessage(waiting_message);

    action();

    // Suppress warning, it's just how Qt works
    app.processEvents(); // NOLINT(readability-static-accessed-through-instance)
}

void ar::helpers::qt::show_error_message_box(const QString& title, const QString& message,
                                             const QString& detailed_text, QSize size, QWidget* parent)
{
    QMessageBox* message_box = new QMessageBox(parent);
    message_box->setIcon(QMessageBox::Icon::Critical);
    message_box->setWindowTitle(title);
    message_box->setText(message);
    message_box->setInformativeText(detailed_text);
    // TODO: On windows user setDetailedText (?)
    message_box->setFixedSize(size.width(), size.height());

    message_box->exec();
}


bool ar::helpers::qt::resources::check_if_resource_exists(const helpers::qt::qt_resource_path& relative_path_to_root)
{
    QString path = ":/" + QString::fromStdString(relative_path_to_root);

    return QFile::exists(path);
}

