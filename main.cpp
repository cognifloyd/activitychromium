#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>
#include <KLocale>
#include <KMainWindow>
#include <KConfigGroup>
#include <KProcess>
//#include <KMessageBox>
#include <iostream>

const char* chromebin_path = "/usr/bin/chromium";

class MainWindow : public KMainWindow
{
  public:
    MainWindow(QString profile = "", QWidget *parent=0);
  protected:
    void saveProperties(KConfigGroup& );
    void readProperties(const KConfigGroup& );
  private:
    QString chromeprofile;
    KProcess p;
};

MainWindow::MainWindow(QString profile, QWidget *parent) : KMainWindow(parent)
{
    setGeometry(100,100,200,100);
    if (profile != "") {
        chromeprofile = ".config/chromium-k-" + profile;
        QStringList createProfileCommand;
        //Create the activity specific profile directory if it doesn't exist
        createProfileCommand << "/usr/bin/mkdir" << "-p" << chromeprofile;
        KProcess::execute(createProfileCommand);
        //Run Chrome with the activity specific profile
        p << chromebin_path << "--user-data-dir=" + chromeprofile;
        p.start();
    } else {
        chromeprofile = "";
    }
}

void MainWindow::saveProperties(KConfigGroup& conf) {
    conf.writeEntry("chromeProfile", chromeprofile);
    p.kill();
}

void MainWindow::readProperties(const KConfigGroup& conf) {
    chromeprofile = conf.readEntry("chromeProfile", QString());
    p << chromebin_path << "--user-data-dir=" + chromeprofile;
    p.start();
}

int main (int argc, char *argv[])
{
  KAboutData aboutData( "activitychromium", 0,
      ki18n("ActivityChromium"), "0.1",
      ki18n("Help Chromium work with KDE activities."),
      KAboutData::License_GPL,
      ki18n("Copyright (c) 2012 Yuen Hoe (Jason moofang); (c) 2015 Jacob Floyd (cognifloyd)") );
  KCmdLineArgs::init( argc, argv, &aboutData );
  KCmdLineOptions options;
  options.add("+profilename", ki18n("Chromium profile name"));
  KCmdLineArgs::addCmdLineOptions(options);
  if (argc <= 1) {
      std::cout << "Please specify Chromium profile name" << std::endl;
      return 0;
  }
 
  KApplication app;
  if ( app.isSessionRestored() ) {
    kRestoreMainWindows< MainWindow >();
  } else {
    // create default application as usual
    // example:
    MainWindow * window = new MainWindow(argv[1]);
    window->setObjectName("MyWindow#");
    window->show();
  }
 
  return app.exec();
}
