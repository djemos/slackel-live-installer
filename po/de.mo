��    >        S   �      H  N   I     �  ;   �  "  �     �  M   }  I   �  �        �  2   �  )   *	     T	     l	    {	     �
     �
     �
     �
  �   �
  ~   �  T   I  D   �     �  h   �  �  [     1     A     N     b     t  !  �  
   �  [   �       G   /  G   w     �     �     �     �  0   �  5     2   K  ;   ~  ,   �  5   �          /     4     9     >     P     T     ]  �   k       	             +     0     8  �  <  T   �     9  6   ?  I  v  �   �  c   Z  C   �  �        �  N   �  2         S     p  �   �     �     �     �     �  �   �  �   �  T   J  W   �     �  u     �  �     q      �      �      �      �     �      	"  X   "     n"  `   �"  c   �"     M#     S#     Y#     t#  .   z#  3   �#  /   �#  :   $  /   H$  :   x$     �$     �$     �$     �$     �$     �$     �$     �$  �   %     �%     �%     �%     �%     �%     �%            '                          )   /      >   7                    =                       !          1   2   *      ,   &                                   :       0   ;       #   -   8                          +   %   3   
   9   .       	             4   6   <   "   (              $       5    <i>Warning: copy / installation wipes everything on the selected location.</i> About Actions needed before proceeding with system installation.
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
to backup important data before starting this procedure. Install (e)LiLo Install Grub Install live system Install location: Installation completed. On Linux systems, the superuser, or root, is a special user
account used for system administration. Here you must set
its password or passphrase. Remember, this password or
passphrase should include a mix of upper and lower case
letters, numbers, and even symbols (such as the @, !, and &) Partitions Set root password, define login name, choose a password to be coupled with your
login name. Slackel-Live Installation Your two root password entries do not match. Please verify and correct. Your two user password entries do not match. Please verify and correct. _File _Help basic installation btrfs check this if you want to format /home partition check this if you want to install (e)Lilo boot loader check this if you want to install Grub boot loader choose the Linux partition to hold the main directory tree. click here if you want to copy system to usb click here if you want to install system on hard disk core installation ext2 ext3 ext4 full installation jfs reiserfs root password root password, user name and user password entries 
cannot be emtpy.
root password and user password should have at least 5 characters

Please verify and correct.
 sli user name user password vfat visible xfs Project-Id-Version: slackel-live-installer
Report-Msgid-Bugs-To: 
PO-Revision-Date: 2019-06-28 18:07+0300
Last-Translator: Dimitris Tzemos <dijemos@gmail.com>
Language-Team: German (http://www.transifex.com/djemos/slackel-live-installer/language/de/)
MIME-Version: 1.0
Content-Type: text/plain; charset=UTF-8
Content-Transfer-Encoding: 8bit
Language: de
Plural-Forms: nplurals=2; plural=(n != 1);
X-Generator: Poedit 2.0.4
 <i>Warnung: Kopie / Installation löscht alles vom ausgewählten Speichermedium.</i> Über Benötigte Aktionen vor der Installation des Systems.
 Basis-Desktop-Installation:
Installiert nur eine einfache grafische Oberfläche mit sehr wenigen
zusätzlichen Anwendungen, wie z.B. einen Webbrowser und die
Paketverwaltung gslapt. Ideal für fortgeschrittene Nutzer, die ein
leichtgewichtiges grafisches System und nachträglich die
Anwendungen ihrer Wahl installieren möchten. GRUB kann nicht auf einem XFS-Dateisystem installiert werden. Das System wird nicht booten.
Stattdessen (e)LiLo wählen. Bitte um Prüfung und Korrektur. Beide Urlader, (e)LiLo und GRUB, können nicht installiert werden.
Bitte um Prüfung und Korrektur. Auswählen, um das Passwort während der Eingabe sichtbar zu machen Passwort oder Passphrase für den Benutzernamen eingeben.
Diese(s) sollte eine Mischung von Groß- und Kleinbuchstaben,
Ziffern und auch Symbolen (wie z.B. @, !, und &) enthalten. Installationsmodus wählen Auswahl des Dateisystemtyps, mit dem die Partition(en) formatiert wird/werden. Dateisystemtyp zum Formatieren des USB auswählen. Live-System auf USB kopieren Speichermedium kopieren: Grundinstallation:
Enthält nur das Minimum an Paketen für ein Kommandozeilensystem
ohne eine grafische Umgebung. Ideal für erfahrene Nutzer, die ihre
Installation für spezielle Zwecke einrichten möchten wie z.B. einen
Webserver, Dateiserver etc. Fehler aufgetreten. Fehler: System ist nicht live. Dateisystemtyp: Home formatieren Volle Installation:
Installiert alles, was auf Installationsmedium enthalten ist. Das ist
eine komplette grafische Desktop-Umgebung
und eine komplette Auswahl passender Anwendungen,
immer dem Konzept "eine Anwendung pro Aufgabe" folgend. Hier muss ein Benutzername definiert werden, der nur alphanumerische
Zeichen enthalten darf, ohne Leerzeichen oder Großbuchstaben. Hier muss das Root-Passwort erneut eingegeben werden,
um die Eingabe zu bestätigen. Hier muss das Benutzerpasswort erneut eingegeben werden,
um die Eingabe zu bestätigen. Speichermedium für Home: Falls für die Home-Partition ein anderes Speichermedium verwendet wird, eines hier wählen.
Andernfalls leer lassen. Wichtig!

Slackel braucht zumindest eine Swap-Partition und eine
Linux-Partition für den Hauptverzeichnisbaum.

Falls diese Partitionen nicht bereits vorhanden sind, sollte jetzt die
Schaltfläche 'Partitionen' angeklickt werden, um sie anzulegen.
Dabei ist zu beachten, dass das Partitionieren eines Speichermediums
eine heikle Operation ist, die bei Nachlässigkeit zu Datenverlust führen
kann. Deshalb wird vor Beginn dieser Prozedur dringend eine Sicherung
wichtiger Daten empfohlen. Installiere (e)LiLo GRUB installieren Live-System installieren Speichermedium installieren: Installation abgeschlossen. Auf Linux-Systemen ist der Superuser oder Root ein spezielles
Benutzerkonto, das zur Administration dient. Hier muss dessen
Passwort oder Passphrase eingegeben werden. Auch dieses sollte
eine Mischung von Groß- und Kleinbuchstaben, Ziffern und
Symbolen (wie z.B. @, !, und &) enthalten. Partitionen Root-Passwort einrichten, Login-Namen definieren, Passwort für den Login-Namen wählen. Slackel-Live-Installation Die beiden Root-Passwörter stimmen nicht miteinander überein.
Bitte um Prüfung und Korrektur. Die beiden Benutzerpasswörter stimmen nicht miteinander überein.
Bitte um Prüfung und Korrektur. _File _Help Basis-Desktop-Installation btrfs Auswählen zum Formatieren der /home-Partition Auswählen, um den Urlader (e)LiLo zu installieren. Auswählen, um den Urlader GRUB zu installieren Auswahl der Linux-Partition für den Hauptverzeichnisbaum. Hier klicken, um das System auf USB zu kopieren Hier klicken, um das System auf Festplatte zu installieren Grundinstallation ext2 ext3 ext4 Volle Installation jfs reiserfs Root-Passwort Die Felder Root-Passwort, Benutzername und Benutzerpasswort
dürfen nicht leer sein.
Root- und Benutzerpasswort sollten aus mindestens 5 Zeichen bestehen.

Bitte um Prüfung und Korrektur.
 sli Benutzername Benutzerpasswort vfat sichtbar xfs 