��    >        S   �      H  N   I     �  ;   �  "  �     �  M   }  I   �  �        �  2   �  )   *	     T	     l	    {	     �
     �
     �
     �
  �   �
  ~   �  T   I  D   �     �  h   �  �  [     1     A     N     b     t  !  �  
   �  [   �       G   /  G   w     �     �     �     �  0   �  5     2   K  ;   ~  ,   �  5   �          /     4     9     >     P     T     ]  �   k       	             +     0     8  �  <  S   �  	   7  4   A  *  v  �   �  `   )  F   �  �   �     �  K   �  C   �  &   2     Y  E  s     �  .   �          !  �   0  �     A   �  :   �  #     k   C  �  �     �      �      �      �      �   �   �   
   �!  {   "     }"  T   �"  [   �"     K#     T#     Z#     o#  7   u#  6   �#  4   �#     $  :   6$  ?   q$     �$     �$     �$     �$     �$     �$     �$      %  �   %     �%     �%     �%     &     &     &            '                          )   /      >   7                    =                       !          1   2   *      ,   &                                   :       0   ;       #   -   8                          +   %   3   
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
PO-Revision-Date: 2019-06-28 18:06+0300
Last-Translator: Dimitris Tzemos <dijemos@gmail.com>
Language-Team: French (http://www.transifex.com/djemos/slackel-live-installer/language/fr/)
MIME-Version: 1.0
Content-Type: text/plain; charset=UTF-8
Content-Transfer-Encoding: 8bit
Language: fr
Plural-Forms: nplurals=2; plural=(n > 1);
X-Generator: Poedit 2.0.4
 <i>Attention : copier / Installer efface tout à l'emplacement sélectionné !.</i> À propos Actions nécessaires avant d'installer le système.
 Installation basique :
Installe seulement un environnement de bureau avec très peu
d'applications complémentaires, tel qu'un navigateur Internet
et le gestionnaire de paquets gslapt. Idéal pour des utilisateurs
avancés voulant un système léger auquel ajouter leur propre
choix d'applications. Impossible d'installer l'amorceur GRUB sur un système de fichiers XFS.
Choisissez (e)LiLo à la place. Merci de vérifier et corriger. Impossible d'installer à la fois les amorceurs (e)LiLo et Grub. Merci de vérifier et corriger. Cochez cette boite si vous voulez voir le mot de passe
que vous tapez. Choisissez un mot de ou une phrase de passe associée à
votre identifiant ce connexion, mélange de lettres capitales
ou non, de chiffres et même de symboles, comme @, !, ou & Choisir le mode d'installation Choisir le type de système de fichier pour formater la ou les partitions : Choisir le type de système de fichier pour formater le support USB Copier le système vivant sur clef USB Emplacement de la copie : Installation de l'essentiel :
Seul le minimum nécessaire pour démarrer un`système en
ligne de commande est inclus. Pas d'environnement graphique.
Idéal pour les utilisateurs expérimentés voulant personnaliser
leur installation pour une usage spécifique comme un serveur
Internet ou un serveur de fichiers, par exemple. Une erreur s'est produite. Erreur: il ne s'agit pas d'un système vivant. Type de système de fichier : Formater /home Installation complète :
Tous les logiciels inclus dans l'image ISO sont installés.
Ceci inclut un environnement de bureau complet et une
sélection étendue d'applications associées, selon le
principe d'une application  par tâche. Définissez ici votre identifiant de connexion, contenant seulement
des caractères alphanumériques sans espace ni lettres capitales. Retapez ici le mot de passe de "root" pour confirmer
votre choix. Retapez ici votre mot de passe pour confirmer
votre choix. Emplacement de la partition /home : Pour utiliser une partition sur une autre disque pour /home choisissez en une ici.
Sinon laissez ceci vide. Important !
Slackel nécessite au strict minimum une partition d'échange
ou SWAP et une partition racine de type Linux.

Si ces partitions ne sont pas prêtes, il faut maintenant
cliquer sur le bouton "Partitions" pour les créer.
Attention ! partitionner un disque dur ou SSD est une
opération délicate qui peut faire perdre des données
si elle n'est pas soigneusement effectuée.
Il est vivement recommandé de sauvegarder vos
données importantes avant de commencer. Installer (e)LiLo Installer Grub. Installer le système vivant Emplacement d'installation : Installation terminée. Le super utilisateur ou "root" d'un système Linux est
un identifiant utilisé pour l'administration du système.
Vous devez lui associer un mot ou phrase de passe,
 mélange de lettres capitales ou non, de chiffres et
de symboles comme @, !, ou &. Partitions Définir un mot de passe "root", définir un identifiant de connexion,
choisir un mot de passe associé à cet identifiant. Installation de Slackel-Live Vos deux frappes de mot de passe root diffèrent. Merci de de vérifier et corriger. Vos deux frappes du mot de passe utilisateur diffèrent. Merci de de vérifier et corriger. _Fichier _Aide installation basique btrfs Cochez ceci si vous voulez formater une partition /home Cocher ici si vous voulez installer l'amorceur (e)LiLo Cocher ici si vous voulez installer l'amorceur Grub. Choisir la partition racine. cliquer ici si vous voulez copier le système sur clef USB cliquez ici si vous voulez installer le système sur disque dur installation de l'essentiel ext2 ext3 ext4 installation complète jfs reiserfs mot de passe root le mot de passe root, l'identifiant de connexion et son mot
de passe ne doivent pas être vides.
les mots de passe doivent comporter au moins 5 caractères

Merci de vérifier et de corriger.

 sli Nom d'utilisateur mot de passe de l'utilisateur vfat visible xfs 