#include <QDebug>
#include "loginwindowpopupform.h"
#include "ui_loginwindowpopupform.h"
#include "mainwindow.h"

#include <QMessageBox>
#include <random>

LoginWindowPopUpForm::LoginWindowPopUpForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWindowPopUpForm)
{
    ui->setupUi(this);
    login_success=false;

    //open if you want an overpower testuser!!
    ui->comboBox->addItem("test");
    //open if you want manager;
    ui->comboBox->addItem("Manager");

    query=new QSqlQuery(database);

    //style sheets
    this->setStyleSheet("QLabel#label_error {background-color:rgba(255,30,30,80) ; "
                        "color: red }");
    ui->btn_return->setStyleSheet("background-color:rgba(223,213,165,255) ;"
                                  "color:white ;");

    QPalette sample_palette_error;
    QColor color;
    color.setRgba(0x50FF0000);
    sample_palette_error.setColor(QPalette::Window,color);
    sample_palette_error.setColor(QPalette::WindowText, Qt::red);

    ui->c_error_password_label->setAutoFillBackground(true);
    ui->c_error_password_label->setPalette(sample_palette_error);
    ui->c_error_username_label->setAutoFillBackground(true);
    ui->c_error_username_label->setPalette(sample_palette_error);
    ui->s_error_code_label->setAutoFillBackground(true);
    ui->s_error_code_label->setPalette(sample_palette_error);
    ui->page->setAutoFillBackground(true);
    ui->page_2->setAutoFillBackground(true);
    color.setRgb(0xDFD5A5);
    sample_palette_error.setColor(QPalette::Window,color);

    ui->page->setPalette(sample_palette_error);
    ui->page_2->setPalette(sample_palette_error);

    //error labels
    reset_error_labels();
    //line_edit placeholder
    initialize_lineedits();
    c=NULL;
    s=NULL;

    //tab order
    setTabOrder(ui->c_id,ui->c_password);
    setTabOrder(ui->c_password,ui->c_password2);
    setTabOrder(ui->c_password2,ui->c_id);
    setTabOrder(ui->s_id,ui->s_password);
    setTabOrder(ui->s_password,ui->s_password2);
    setTabOrder(ui->s_password2,ui->s_authorizationcode);
    setTabOrder(ui->s_authorizationcode,ui->s_id);

}
void LoginWindowPopUpForm::signOrLog(enum state s)
{
    reset_error_labels();
    initialize_lineedits();
    if(s==login_)
    {
        ui->lineedit_password->clear();
        ui->stackedWidget->setCurrentIndex(0);
        ui->tabWidget->setTabVisible(0,true);
        ui->tabWidget->setTabVisible(1,false);
        ui->tabWidget->setTabVisible(2,false);
    }
    else
    {
        ui->tabWidget->setTabVisible(0,false);
        ui->tabWidget->setTabVisible(1,true);
        ui->tabWidget->setCurrentIndex(1);
        ui->tabWidget->setTabVisible(2,true);
    }
};
LoginWindowPopUpForm::~LoginWindowPopUpForm()
{
    delete ui;
}
void LoginWindowPopUpForm::on_c_signup_button_clicked()
{
    ui->c_error_username_label->hide();
    ui->c_error_password_label->hide();

    name_in = ui->c_id->text();
    password_in = ui->c_password->text();
    QString password_check =ui->c_password2->text();

    if(!validName(name_in)){
        ui->c_error_username_label->setText("Username invalid.");
        ui->c_error_username_label->show();
    }
    else if(!validPass(password_in))
    {
        ui->c_error_password_label->setText("Password invalid.");
        ui->c_error_password_label->show();
    }
    else if(QString::compare(password_in,password_check))
    {
        ui->c_error_password_label->setText("Passwords don't match.");
        ui->c_error_password_label->show();
        ui->c_password2->clear();
    }
    else if(!c_duplicate_username(name_in)) // no dulplicate
    {
        qDebug()<<"register ok";
        QMessageBox::information(this," ","Congratulation!\nYou are now our member.\nPlease relogin after this message.");
        ui->lineedit_ID->setText(name_in);
        ui->comboBox->setCurrentIndex(0);
        signOrLog(login_);

        query->exec("SELECT COUNT(*) FROM customer_list;");
        query->next();
        int x=query->value(0).toInt();
        int random_generate_id=(int)((rand()/(RAND_MAX+1.)*1000)+1e8+x*1e6);

        //customer pointer is linked with mainwindow
        c=new Customer(random_generate_id,name_in,password_in);

        //sql insert to customer_list
        sql_command="INSERT INTO customer_list VALUES("+QString::number(random_generate_id)+",'"+
                name_in+"','"+password_in+"',1000,100);";
        query->exec(sql_command);
        //sql insert to customer info
        sql_command="INSERT INTO customer_info VALUES('"+name_in+"',NULL,NULL,NULL,NULL);";
        query->exec(sql_command);
        //sql create customer bag
        sql_command="CREATE TABLE c_"+QString::number(random_generate_id)+"(card_name nvarchar(50),card_type varchar(7),card_url varchar(120),count int,star bool);";
        query->exec(sql_command);
    }
    else
    {
        ui->c_error_username_label->setText("That ID is taken.\nPlease try another.");
        ui->c_error_username_label->show();
        ui->c_password->clear();
        ui->c_password2->clear();
        ui->c_id->clear();
        qDebug("error: dulpicate id!\n");
    }
}
void LoginWindowPopUpForm::on_s_signup_button_clicked()
{
    ui->s_error_password_label->hide();
    ui->s_error_username_label->hide();
    ui->s_error_code_label->hide();

    query->exec("SELECT id FROM seller_list WHERE username='manager';");
    query->next();
    realcode=QString::number(query->value(0).toInt()+10000);
    realcode.remove(0,1);
    qDebug()<<realcode;
    name_in= ui->s_id->text();
    password_in = ui->s_password->text();
    QString password_check =ui->s_password2->text();
    QString code=ui->s_authorizationcode->text();

    if(!validName(name_in)){
        ui->s_error_username_label->setText("Username invalid.");
        ui->s_error_username_label->show();
    }
    else if(!validPass(password_in))
    {
        ui->s_error_password_label->setText("Password invaid.");
        ui->s_error_password_label->show();
    }
    else if(QString::compare(password_in,password_check))
    {
        ui->s_error_password_label->setText("Passwords don't match.");
        ui->s_error_password_label->show();
        ui->s_password2->clear();
    }
    else if((QString::compare(code,realcode)))
    {
        ui->s_error_code_label->setText("Please ask manager for Authorization code.");
        ui->s_error_code_label->show();
        ui->s_authorizationcode->clear();
    }
    else if(!s_duplicate_username(name_in))
    {
        qDebug()<<"register ok";
        QMessageBox::information(this," ","Congratulation!\nYou are now our seller.\nPlease relogin after this message.");
        ui->lineedit_ID->setText(name_in);
        ui->comboBox->setCurrentIndex(1);
        signOrLog(login_);
        query->exec("SELECT COUNT(*) FROM seller_list;");
        query->next();
        int x=query->value(0).toInt();
        int random_generate_id=(int)((rand()/(RAND_MAX+1.)*1000)+2e8+x*1e6);

        //pointer is linked with mainwindow
        s=new Seller(random_generate_id,name_in,password_in);

        //sql insert to seller list
        sql_command="INSERT INTO seller_list VALUES("+QString::number(random_generate_id)+",'"+
                name_in+"','"+password_in+"');";
        query->exec(sql_command);
    }
    else
    {
        ui->s_error_username_label->setText("That ID is taken.\nPlease try another.");
        ui->s_error_username_label->show();
        ui->s_password->clear();
        ui->s_password2->clear();
        ui->s_id->clear();
        qDebug("error: dulpicate id!\n");
    }
}
bool LoginWindowPopUpForm::c_duplicate_username(QString username)
{
    sql_command="SELECT COUNT(*) FROM customer_list WHERE username = '"+username+"';";
    query->exec(sql_command);
    query->next();
    return query->value(0).toBool();
}
bool LoginWindowPopUpForm::s_duplicate_username(QString username)
{
    sql_command="SELECT COUNT(*) FROM seller_list WHERE username = '"+username+"';";
    query->exec(sql_command);
    query->next();
    return query->value(0).toBool();
}
void LoginWindowPopUpForm::on_Login_button_clicked()
{
    QString x=ui->comboBox->currentText();
    valid_user=0;
    name_in= ui->lineedit_ID->text();
    qDebug()<<name_in.toInt();
    //using ID to login find in data and translate to username
    if(name_in.toInt()){
        qDebug()<<name_in.toInt()/(int)1e8;
        if(name_in.toInt()/(int)1e8==1){
            query->exec("select username from customer_list where id = "+QString::number(name_in.toInt())+";");
            x="Customer";
        }
        else{
            query->exec("select username from seller_list where id = "+QString::number(name_in.toInt())+";");
            x="Seller";
        }
        if(query->next()){
            name_in=query->value(0).toString();
        }
        else
            name_in='0';
    }
    qDebug()<<name_in;
    if(!QString::compare(x,"Customer"))
    {
        identity=cus;
        if((valid_user=c_duplicate_username(name_in))){
            if(c)
            {
                Customer* t=c;
                delete t;
            }
            retrieve_customer(name_in);
            //implement codes to retrieve basic data for customer;
        }
    }
    else if(!QString::compare(x,"Seller"))
    {
        identity=sel;

        if((valid_user=s_duplicate_username(name_in))){
            if(s)
            {
                Seller* t=s;
                delete t;
            }
            retrieve_seller(name_in);
        }
    }
    else if(!(QString::compare(x,"Manager")||QString::compare(name_in,"manager")))
    {
        identity=man;
        if(s)
        {
            Seller* t=s;
            delete t;
        }
        query->exec("SELECT * FROM seller_list WHERE username = 'manager';");
        query->next();
        int code=query->value(0).toInt();
        QString m_name=query->value("username").toString();
        QString m_pass=query->value("password").toString();
        s=new Manager(0,m_name,m_pass,code);
        valid_user=1;
    }
    else if(!(QString::compare(x,"test")||QString::compare(name_in,"test")))
    {
        identity=test;
        valid_user=1;
    }
    if(valid_user)
    {
        if(identity==cus)
            ui->label_identity->setText("Login as Customer...");
        else if(identity==sel)
            ui->label_identity->setText("Login as Seller...");
        else if(identity==man)
            ui->label_identity->setText("Login as Manager...");
        else
            ui->label_identity->setText("Test...");
        ui->stackedWidget->setCurrentIndex(1);
    }
    else
    {
        if(validName(name_in)){
            ui->label_error->setText("Account does not exist.\nDo you want to Sign up?");
            ui->label_error->show();
            ui->commandLinkButton->show();
        }
        else
        {
            ui->label_error->setText("Invalid Account or ID.");
            ui->label_error->show();
        }
    }
}
void LoginWindowPopUpForm::retrieve_customer(QString user)
{
    sql_command="SELECT * FROM customer_list WHERE username = '"+user+"';";
    query->exec(sql_command);
    //qDebug()<<query->size();
    query->next();

    int id=query->value(0).toInt();
    QString pass=query->value(2).toString();
    int cash=query->value(3).toInt();
    double point=query->value(4).toDouble();
    c=new Customer(id,user,pass,cash,point);
}
void LoginWindowPopUpForm::retrieve_seller(QString user)
{
    sql_command="SELECT * FROM seller_list WHERE username = '"+user+"';";
    query->exec(sql_command);
    query->next();
    int id=query->value(0).toInt();
    QString pass=query->value(2).toString();
    s=new Seller(id,user,pass);
}
void LoginWindowPopUpForm::on_lineedit_ID_returnPressed()
{
    on_Login_button_clicked();
}
void LoginWindowPopUpForm::on_Login_button_pass_clicked()
{
    password_in=ui->lineedit_password->text();

    if(identity==cus&&c->login(password_in))
    {
        login_success=true;
        emit cusLoggedin();
        close();
    }
    else if(identity==sel&&s->login(password_in))
    {

        login_success=true;
        emit selLoggedin();
        close();
    }
    else if(identity==man&&s->login(password_in))/*!QString::compare(password_in,"manager")*/
    {
        login_success=true;
        emit manLoggedin();
        close();
    }
    else if(identity==test&&!QString::compare(password_in,"test"))
    {
        login_success=true;
        emit testLoggedin();
        close();
    }
    else
    {
        ui->label_error_pass->setText("Password error, please try again.");
        ui->label_error_pass->show();
    }
}
void LoginWindowPopUpForm::on_lineedit_password_returnPressed()
{
    on_Login_button_pass_clicked();
}
bool LoginWindowPopUpForm::validPass(QString pass){
    bool contain_english=false,contain_num=false;
    if(pass.length()<6||pass.length()>12)
        return false;
    for(auto& c:pass)   {
        if(c>='0'&&c<='9'){contain_num=true;}
        else if(c>='a'&&c<='z'){contain_english=true;}
        else if(c>='A'&&c<='Z'){contain_english=true;}
        else
            return false;
    }
    return contain_english&&contain_num;
}

bool LoginWindowPopUpForm::validName(QString name){
    bool contain_english=false,contain_num=false;
    if(name.length()<6||name.length()>12)
        return false;
    for(auto& c:name)   {
        if(c>='0'&&c<='9'){contain_num=true;}
        else if(c>='a'&&c<='z'){contain_english=true;}
        else if(c>='A'&&c<='Z'){contain_english=true;}
        else if(c=='.'||c=='_');
        else
            return false;
    }
    return contain_english&&contain_num;
}
void LoginWindowPopUpForm::on_btn_return_clicked()
{
    ui->label_identity->clear();
    reset_error_labels();
    ui->lineedit_password->clear();
    ui->lineedit_ID->clear();
    ui->stackedWidget->setCurrentIndex(0);
}

void LoginWindowPopUpForm::on_lineedit_ID_selectionChanged()
{
    ui->lineedit_ID->setReadOnly(0);
}

//sign up with a valid unused Username
void LoginWindowPopUpForm::on_commandLinkButton_clicked()
{
    signOrLog(signup_);
    ui->s_password->setFocus();
    ui->c_id->setText(name_in);
    ui->c_password->setFocus();
    ui->s_id->setText(name_in);

}
//already has account
void LoginWindowPopUpForm::on_commandLinkButton_2_clicked()
{
    signOrLog(login_);
}
void LoginWindowPopUpForm::on_commandLinkButton_3_clicked()
{
    signOrLog(login_);
}

void LoginWindowPopUpForm::on_to_main_btn_clicked()
{
    close();
    mypage->setCurrentIndex(prePage);
}

void LoginWindowPopUpForm::on_lineedit_password_selectionChanged()
{
    ui->lineedit_password->setEchoMode(QLineEdit::Password);
    ui->lineedit_password->setReadOnly(0);
}
void LoginWindowPopUpForm::on_toolbtn_c_revealPass_pressed()
{
    ui->c_password->setEchoMode(QLineEdit::Normal);
}
void LoginWindowPopUpForm::on_toolbtn_c_revealPass_released()
{
     ui->c_password->setEchoMode(QLineEdit::Password);
}
void LoginWindowPopUpForm::on_toolbtn_c_revealPass2_pressed()
{
    ui->c_password2->setEchoMode(QLineEdit::Normal);
}
void LoginWindowPopUpForm::on_toolbtn_c_revealPass2_released()
{
 ui->c_password2->setEchoMode(QLineEdit::Password);
}
void LoginWindowPopUpForm::on_toolbtn_s_revealPass_pressed()
{
    ui->s_password->setEchoMode(QLineEdit::Normal);
}
void LoginWindowPopUpForm::on_toolbtn_s_revealPass_released()
{
     ui->s_password->setEchoMode(QLineEdit::Password);
}
void LoginWindowPopUpForm::on_toolbtn_s_revealPass2_pressed()
{
    ui->s_password2->setEchoMode(QLineEdit::Normal);
}
void LoginWindowPopUpForm::on_toolbtn_s_revealPass2_released()
{
    ui->s_password2->setEchoMode(QLineEdit::Password);
}

void LoginWindowPopUpForm::reset_error_labels()
{
    ui->label_error->clear();
    ui->label_error_pass->clear();
    ui->c_error_password_label->clear();
    ui->c_error_username_label->clear();
    ui->s_error_password_label->clear();
    ui->s_error_username_label->clear();
    ui->label_error->hide();
    ui->label_error_pass->hide();
    ui->c_error_password_label->hide();
    ui->c_error_username_label->hide();
    ui->s_error_password_label->hide();
    ui->s_error_username_label->hide();
    ui->s_error_code_label->hide();
}
void LoginWindowPopUpForm::initialize_lineedits()
{
    //set initial display of login
    ui->lineedit_ID->setPlaceholderText("Account or ID");
    ui->lineedit_ID->setReadOnly(1);
    ui->lineedit_password->setPlaceholderText("Password");
    ui->lineedit_password->setReadOnly(1);
    ui->commandLinkButton->hide();
    ui->stackedWidget->setCurrentIndex(0);


    //signup tabs echomode
    ui->c_password->clear();
    ui->c_password2->clear();
    ui->s_password->clear();
    ui->s_password2->clear();
    ui->s_authorizationcode->clear();
    ui->c_password->setEchoMode(QLineEdit::Password);
    ui->c_password2->setEchoMode(QLineEdit::Password);
    ui->s_password->setEchoMode(QLineEdit::Password);
    ui->s_password2->setEchoMode(QLineEdit::Password);
    ui->s_authorizationcode->setEchoMode(QLineEdit::Password);


}
void LoginWindowPopUpForm::on_tabWidget_tabBarClicked(int index)
{
    if(index==1){
        if(!ui->s_id->text().isEmpty()&&QString::compare(ui->s_id->text(),ui->c_id->text())){
            ui->c_id->clear();
        }
        ui->c_password->clear();
        ui->c_password2->clear();
    }
    if(index==2){
        if(!ui->c_id->text().isEmpty()&&QString::compare(ui->s_id->text(),ui->c_id->text())){
            ui->s_id->clear();
        }
        ui->s_password->clear();
        ui->s_password2->clear();
    }
}
void LoginWindowPopUpForm::logout(){
    login_success=false;
    reset_error_labels();
}




void LoginWindowPopUpForm::on_Hidden_butt_clicked()
{
    name_in="0710849d0108";
    retrieve_customer(name_in);
    ui->lineedit_password->setText("f951235789");
    identity=cus;
    on_Login_button_pass_clicked();
}

