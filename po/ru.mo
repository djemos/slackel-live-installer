��    M      �  g   �      �  N   �     �  ;   �  "       =  M   �  I   	  �   U	     
  2   7
  )   j
     �
     �
    �
     �     �            �   %  ~   
  T   �  D   �     #  h   2  �  �     q     �     �     �     �     �     �  !  �  
   �  &   	  3   0     d  $   �  [   �       G     G   g     �     �     �     �     �  0   �  5     2   B  ;   u  ,   �  5   �          &     -     2     7     <     N     R     [     h     v     {     �     �     �  �   �     D     H  	   [     e     s     x     �  2  �  t   �     ,  O   B  �  �  �   f  }      U   �  �   �  .   �  U   �  H   R  ;   �     �  �  �  "   �   /   �       &!  8   G!  9  �!  �   �"  �   m#  �   $  1   �$  �   �$    `%     b(     (  0   �(     �(  &   �(     
)     )  �   )     �*  I   �*  C    +  1   d+  -   �+  j   �+  !   /,  T   Q,  h   �,  	   -     -     )-  !   0-     R-  I   X-  G   �-  D   �-  O   /.  _   .  V   �.  %   6/     \/     i/     n/     s/     x/     �/     �/  '   �/     �/     �/     �/     0     0     0    #0     C1     G1     T1  %   t1     �1     �1     �1              G   H   )   ,                      M   .      ?   K   @   *   (       J   9   :      +          %      ;   <            >   5                  '   =                       I   4       !   2   6                             
          E         L          	              F   &   8   D   3           /   7   B      -      C       $   1       0             #   A      "        <i>Warning: copy / installation wipes everything on the selected location.</i> About Actions needed before proceeding with system installation.
 Basic installation:
This installs only a basic desktop environment, with very few 
extra applications installed on top, such as a web browser and
the gslapt package manager. Ideal for advanced users that 
would like to install a lightweight system and add their own
choice of applications.  Cannot install Grub boot loader on an xfs filesystem. System will not boot. 
Choose (e)Lilo instead. Please verify and correct. Cannot install both (e)Lilo and Grub boot loaders. Please verify and correct. Check this box if you want to be able to see the password
you are typing. Choose a password or passphrase to be coupled with your
login name. Your password or passprase should include a mix
of upper and lower case letters, numbers, and even symbols
(such as the @, !, and &) Choose installation mode Choose the filesystem type to format partition(s). Choose the filesystem type to format usb. Copy live system to usb Copy location: Core installation:
Only the minimum essentials for a console system to start are 
included. A graphical environment is not provided. This is ideal 
if you are an experienced user and want to customize your 
installation for any specific purpose, such as a web server.
file server etc.  Error encountered. Error: system is not live. Filesystem Type: Format Ηome Full installation:
Everything that is included in the iso is installed. That includes 
a complete desktop environment
and a complete selection of matching applications,
always following the "one application per task" rationale.  Here you must define your login name which should only include
alphanumerical characters with no space or upper case letters.  Here you must retype the superuser (root) password as a
confirmation of your choice. Here you must retype your password as
a confirmation of your choice. Home location: If you want to use a home partition to another disk then choose one from here.
Otherwise leave it empty. Important!

Slackel needs, at the very minimum, one swap partition
and one Linux partition to hold its main directory tree.

If you do not already have these needed partitions ready, 
you should now click on the button 'Partitions'
in order to create them. Be aware that partitioning a disk
drive is a delicate operation which could lead to data loss
if not performed cautiously. It is therefore highly advisable
to backup important data before starting this procedure. Install (e)LiLo Install Grub Install live system Install location: Installation completed. Language Locale On Linux systems, the superuser, or root, is a special user
account used for system administration. Here you must set
its password or passphrase. Remember, this password or
passphrase should include a mix of upper and lower case
letters, numbers, and even symbols (such as the @, !, and &) Partitions Please select a default system locale. Please select your preferred keyboard type and map. Select a default system locale. Select your preferred keyboard type. Set root password, define login name, choose a password to be coupled with your
login name. Slackel-Live Installation Your two root password entries do not match. Please verify and correct. Your two user password entries do not match. Please verify and correct. _File _Help azerty basic installation btrfs check this if you want to format /home partition check this if you want to install (e)Lilo boot loader check this if you want to install Grub boot loader choose the Linux partition to hold the main directory tree. click here if you want to copy system to usb click here if you want to install system on hard disk core installation dvorak ext2 ext3 ext4 full installation jfs keyboard keyboard map keyboard type olpc qwerty qwertz reiserfs root password root password, user name and user password entries 
cannot be emtpy.
root password and user password should have at least 5 characters

Please verify and correct.
 sli translator-credits user name user password vfat visible xfs Project-Id-Version: slackel-live-installer
Report-Msgid-Bugs-To: 
PO-Revision-Date: 2019-06-28 18:08+0300
Last-Translator: Dimitris Tzemos <dijemos@gmail.com>
Language-Team: Russian (http://www.transifex.com/djemos/slackel-live-installer/language/ru/)
MIME-Version: 1.0
Content-Type: text/plain; charset=UTF-8
Content-Transfer-Encoding: 8bit
Language: ru
Plural-Forms: nplurals=4; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<12 || n%100>14) ? 1 : n%10==0 || (n%10>=5 && n%10<=9) || (n%100>=11 && n%100<=14)? 2 : 3);
X-Generator: Poedit 2.0.4
 <i>Внимание: копирование/установка удалят текущее содержимое.</i> О программе Прежде чем приступить к установке системы.
 Базовая установка:
Устанавливается базовое окружение рабочего стола и
несколько приложений, таких как браузер и пакетный 
менеджер gslapt. Подойдет продвинутым пользователям,
желающим установить облегчённую систему и добавить
приложения по своему выбору. Невозможно установить загрузчик GRUB на xfs. Система не загрузится.
Выберите (e)LiLo. Проверьте и исправьте. Невозможно установить (e)LiLo и GRUB одновременно. Проверьте и исправьте. Отметьте, если хотите видеть введённые пароли. Выберите пароль для пользователя. Пароль должен
содержать строчные и заглавные буквы, цифры и
символы (например, @, !, &). Выберите режим установки Файловая система для форматирования разделов. Файловая система для форматирования USB. Скопировать 'живую' систему на USB Копировать в: Урезанная установка:
Устанавливается только минимально необходимое для
консольной системы. Графическое окружение отсутствует.
Подойдёт опытным пользователям, настраивающим
систему для конкретных целей, например, в качестве
вебсервера, файлового сервера и т.п. Обнаружена ошибка. Ошибка: система не 'живая'. Файловая система: Форматировать домашний раздел Полная установка:
Устанавливается всё, что есть в ISO. Сюда входят полное
окружение рабочего стола и полный набор приложений,
следующих принципу 'одно приложение на задачу'. Здесь необходимо задать имя пользователя только из 
строчных латинских букв, цифр и без пробелов. Здесь нужно повторить пароль суперпользователя (root)
для исключения опечаток при наборе. Здесь нужно повторить пароль пользователя
для исключения опечаток при наборе. Раздел домашних каталогов: Выберите отдельный раздел для домашних каталогов.
Либо оставьте поле пустым. Внимание!

Для установки Slackel требуются как минимум раздел
подкачки (swap) и раздел Linux для корневого каталога.

Если у вас такие разделы ещё не подготовлены, создать
их сейчас возможно щёлкнув по кнопке 'Разделы'. Помните,
что разметка диска -- это деликатная операция, способная
в случае неправильных действий привести к потере данных.
Поэтому настоятельно рекомендуем предварительно
сделать резервные копии важных данных. Установить (e)LiLo Установить GRUB Установить 'живую' систему Установить в: Установка завершена. Язык Локаль В системах Linux суперпользователь root является
специальной учетной записью, используемой для
администрирования системы. Здесь нужно задать
пароль. Пароль должен содержать строчные и
заглавные буквы, цифры и символы (например, @, !, &). Разделы Выберите системную локаль по умолчанию. Выберите тип и раскладку клавиатуры. Выберите системную локаль. Выберите тип клавиатуры. Задайте пароль для root, 
имя пользователя и пароль для него. Установщик Slackel-Live Пароли root не совпадают. Проверьте и исправьте. Пароли пользователя не совпадают. Проверьте и исправьте. _Файл _Справка azerty базовая установка btrfs отметьте для форматирования раздела /home отметьте для установки загрузчика (e)LiLo отметьте для установки загрузчика GRUB выберите раздел Linux для корневого каталога. щёлкните здесь, чтобы скопировать систему на флешку щёлкните для установки системы на жёсткий диск урезанная установка Дворак ext2 ext3 ext4 полная установка jfs Клавиатура раскладка клавиатуры тип клавиатуры olpc qwerty qwertz reiserfs пароль root Поля пароль root, имя пользователя и пароль пользователя
не могут быть пустыми.
Пароли root и пользователя не могут быть короче 5 символов.

Поверьте и исправьте.
 sli Serg Bormant имя пользователя пароль пользователя vfat показать xfs 