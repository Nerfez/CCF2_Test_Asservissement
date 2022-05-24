// Test_Asservissement_CLR_1.cpp : fichier projet principal.

// Mise à jour : 24/04/2022
///////////////////////////

///////////////////////////////////
//                               // 
//  Base de travail provisoire : //
//                               // 
//   à compléter et organiser    // 
//    suivant les nécessités     //  
//  de l'application développée  //
//                               //
///////////////////////////////////

// Fichier projet principal.

#include "stdafx.h"

//Espace de noms additionnels
using namespace System::Xml; // Classes de gestion de documents Xml

// Espaces de noms initiaux
///////////////////////////

using namespace System;

// Espaces de noms additionnels
///////////////////////////////

using namespace Threading;
//////////////////////////

// Espace de noms du module LabJack U12

using namespace lj;
///////////////////

// Fonction principale
//////////////////////

int main(array < System::String ^ > ^ args) {

  // Affichage provisoire

  Console::WriteLine("Module U12 : Test CCF 2");
  ///////////////////////////////////////////////////

  // Variables locales
  ////////////////////

  //Création d'un tableau de 10 échantillons pour la 1ere seconde de relevé
  float enchantillons_sortie[10];

  float r = 0.0;

  int compteur = 0;

  String ^ h_chaîne;

  h_chaîne = gcnew String("Néant");

  bool validité;

  // Instanciation de la classe
  //  de gestion du module U12
  //////////////////////////////

  LabJack ^ h_lab_jack_1;

  h_lab_jack_1 = gcnew LabJack();
  /////////////////////////////////

  // Appel de la méthode GetDriverVersion

  r = h_lab_jack_1 -> GetDriverVersion();
  /////////////////////////////////////////

  // Affichage provisoire

  Console::WriteLine("Version : {0:n} ", r);
  ////////////////////////////////////////////////

  ////////////////////////////////////////////////////
  // Essai 1 : écritures vers une sortie analogique //
  ////////////////////////////////////////////////////

  int numéro_série;

  int type_opération = 0;

  float tension_sortie_0 = 0.0 f;

  float tension_sortie_1 = 0.0 f;

  //Arrêt du moteur
  h_lab_jack_1 -> EAnalogOut(numéro_série, type_opération, tension_sortie_0, tension_sortie_1);
  h_lab_jack_1 -> EAnalogOut(numéro_série, type_opération, tension_sortie_0, tension_sortie_1);

  //Temporisation
  Thread::Sleep(2000);

  // Saisie d'une tension 1

  Console::Write("Entrer une tension 1 de 0 à 5 Volts : \r\n");
  //////////////////////////////////////////////////////////////////

  h_chaîne = Console::ReadLine();
  ///////////////////////////////////

  // Conversion sécurisée

  validité = float::TryParse(h_chaîne, tension_sortie_0);
  /////////////////////////////////////////////////////////////

  // Ecriture de la tension 1

  if (validité == true) {
    /*************************************************

     Méthode statique EAnalogOut ( )

     Paramètres:
    	
    	Entrée:

    		- type_opération : 0 si opération normale

    		- tension_sortie_0 : Sortie Analogique 0			
    		- tension_sortie_1 : Sortie Analogique 1
    
    	Sortie:

    		- numéro_série : LabJack détecté

    **************************************************/

    h_lab_jack_1 -> EAnalogOut(numéro_série, type_opération, tension_sortie_0, tension_sortie_1);
    ///////////////////////////////////////////////////////////////////////////////////////////////////

    Console::WriteLine("DAC 0 valeur 1 : {0:0.###} Volts\r\n", tension_sortie_0);
    ///////////////////////////////////////////////////////////////////////////////////
  }

  // Affichage provisoire

  if (validité == false) {
    Console::WriteLine("Saisie incorrecte : {0:0.###}\r\n", tension_sortie_0);
    ////////////////////////////////////////////////////////////////////////////////
  }

  // Essai 2 : lectures depuis une entrée analogique
  //////////////////////////////////////////////////

  int canal = 0;

  int gain = 0;

  int surtension;

  float tension_lue;

  /****************************************************************

	 Méthode statique EAnalogIn ( )

	 Paramètres:
		
		Entrée:

			- type_opération : 0 si opération normale

			- canal d'entrée à lire : 0 à 7  , entrée simple
                                      8 à 11 , entrée différentielle
									  
			- commande du gain : 0 si gain de 1
			                     1 si gain de 2
								 ...............
								 7 si gain de 20
		Sortie:

			- numéro_série : LabJack détecté

			- surtension : 0 ou valeur anormale détectée sur l'entrée 
			- tension_lue : retourne la valeur de tension lue

	******************************************************************/

  while (compteur < 10) {
    h_lab_jack_1 -> EAnalogIn(numéro_série, type_opération, canal, gain, surtension, enchantillons_sortie[compteur]);
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    Console::WriteLine("AI0 valeur lue : {0:0.###} Volts\r", enchantillons_sortie[compteur]);
    ////////////////////////////////////////////////////////////////////////////

    compteur += 1;

    // Temporisation provisoire

    Thread::Sleep(100);
    ////////////////////////
  }

  tension_sortie_0 = 0.0 f;
  tension_sortie_1 = 0.0 f;

  //Arrêt moteur
  h_lab_jack_1 -> EAnalogOut(numéro_série, type_opération, tension_sortie_0, tension_sortie_1);

  //////////////////////////////////////////////////////////////////////////////
  //Création du fichier xml

  //10 echantillon que l'on place à une valeur inférieur à la normale pour voir
  //comment réagit le XML
  enchantillons_sortie[10] = -15.4;

  float mesure = 0.0 f;

  //Handle sur la classe DateTime
  DateTime ^ h_date_time;

  //Handle sur la classe XmlWriter
  XmlWriter ^ h_Ecriture_Document_XML;

  //Handle sur la classe XmlWriterSettings
  XmlWriterSettings ^ h_Paramètres_Document_XML;

  //Création d'un objet de paramétrage du document XML
  h_Paramètres_Document_XML = gcnew XmlWriterSettings();

  //Indentation pour chaque balise 
  h_Paramètres_Document_XML -> Indent = true;

  //Indentation d'une tabulation pour le balisage
  h_Paramètres_Document_XML -> IndentChars = ("   ");

  //Création du document XML
  try {
    //Création du fichier
    //Création du fichier avec les paramètres précédemment établis
    //incluant la génération d'un en-tête conforme au format XML

    h_Ecriture_Document_XML = XmlWriter::Create("Relevé_1.xml", h_Paramètres_Document_XML);

    //Début de l'écriture de données dans le fichier
    h_Ecriture_Document_XML -> WriteStartDocument();

    //Début de la balise racine
    h_Ecriture_Document_XML -> WriteStartElement("Relevés");

    //Ouverture d'une balise date
    h_Ecriture_Document_XML -> WriteStartElement("Date");

    //Contenu : nom et affectation d'une valeur
    h_date_time = DateTime::Now;

    h_chaîne = String::Format("{0:d}", h_date_time);
    h_Ecriture_Document_XML -> WriteString(h_chaîne);

    //Fermeture de la balise Date
    h_Ecriture_Document_XML -> WriteEndElement();

    //Ouverture de la balise Heure
    h_Ecriture_Document_XML -> WriteStartElement("Heure");

    //Ecriture du contenu : nom et affectation d'une valeur
    h_chaîne = String::Format("{0:T}", h_date_time);
    h_Ecriture_Document_XML -> WriteString(h_chaîne);

    //Fermeture de la balise Heure
    h_Ecriture_Document_XML -> WriteEndElement();

    //Création de trois noeuds echantillons

    for (int i = 0; i <= 10; i += 1) {
      //Ouverture d'une balise parent Echantillon
      h_Ecriture_Document_XML -> WriteStartElement("Echantillon");

      //Attribut : nom et affectation d'une valeur

      if (i == 0)
        h_Ecriture_Document_XML -> WriteAttributeString("Vitesse", "0");

      if (i == 1)
        h_Ecriture_Document_XML -> WriteAttributeString("Vitesse", "1");

      if (i == 2)
        h_Ecriture_Document_XML -> WriteAttributeString("Vitesse", "2");

      if (i == 3)
        h_Ecriture_Document_XML -> WriteAttributeString("Vitesse", "3");

      if (i == 4)
        h_Ecriture_Document_XML -> WriteAttributeString("Vitesse", "4");

      if (i == 5)
        h_Ecriture_Document_XML -> WriteAttributeString("Vitesse", "5");

      if (i == 6)
        h_Ecriture_Document_XML -> WriteAttributeString("Vitesse", "6");

      if (i == 7)
        h_Ecriture_Document_XML -> WriteAttributeString("Vitesse", "7");

      if (i == 8)
        h_Ecriture_Document_XML -> WriteAttributeString("Vitesse", "8");

      if (i == 9)
        h_Ecriture_Document_XML -> WriteAttributeString("Vitesse", "9");

      if (i == 10)
        h_Ecriture_Document_XML -> WriteAttributeString("Vitesse", "10");

      //Ecriture du contenu : affectation d'exemples de valeurs

      if (enchantillons_sortie[i] < 10.0 && enchantillons_sortie[i] > -10.0) {

        //Ouverture d'une balise enfant Vitesse
        h_Ecriture_Document_XML -> WriteStartElement("Vitesse");

        if (i == 0)
          mesure = enchantillons_sortie[0];

        if (i == 1)
          mesure = enchantillons_sortie[1];

        if (i == 2)
          mesure = enchantillons_sortie[2];

        if (i == 3)
          mesure = enchantillons_sortie[3];

        if (i == 4)
          mesure = enchantillons_sortie[4];

        if (i == 5)
          mesure = enchantillons_sortie[5];

        if (i == 6)
          mesure = enchantillons_sortie[6];

        if (i == 7)
          mesure = enchantillons_sortie[7];

        if (i == 8)
          mesure = enchantillons_sortie[8];

        if (i == 9)
          mesure = enchantillons_sortie[9];

        if (i == 10)
          mesure = enchantillons_sortie[10];

        h_chaîne = String::Format("{0:n}", mesure);

        h_Ecriture_Document_XML -> WriteString(h_chaîne);

        //Fermeture de la balise enfant vitesse
        h_Ecriture_Document_XML -> WriteEndElement();

      } //FIN ECRITURE

      //Ecriture du contenu : affectation d'exemples de valeurs

      if (enchantillons_sortie[i] > 10.0 || enchantillons_sortie[i] < -10.0) {

        //Ouverture de la balise Erreur
        h_Ecriture_Document_XML -> WriteStartElement("Erreur");

        if (i == 0)
          mesure = enchantillons_sortie[0];

        if (i == 1)
          mesure = enchantillons_sortie[1];

        if (i == 2)
          mesure = enchantillons_sortie[2];

        if (i == 3)
          mesure = enchantillons_sortie[3];

        if (i == 4)
          mesure = enchantillons_sortie[4];

        if (i == 5)
          mesure = enchantillons_sortie[5];

        if (i == 6)
          mesure = enchantillons_sortie[6];

        if (i == 7)
          mesure = enchantillons_sortie[7];

        if (i == 8)
          mesure = enchantillons_sortie[8];

        if (i == 9)
          mesure = enchantillons_sortie[9];

        if (i == 10)
          mesure = enchantillons_sortie[10];

        h_chaîne = String::Format("{0:n}", mesure);
        h_Ecriture_Document_XML -> WriteString(h_chaîne);

        //Fermeture balise Erreur
        h_Ecriture_Document_XML -> WriteEndElement();
      }

      //Fermeture de la balise parent Echantillon
      h_Ecriture_Document_XML -> WriteEndElement();

    } //Fin du for

    //Fermeture de la balise racine
    h_Ecriture_Document_XML -> WriteEndElement();

    //Fermeture du document XML
    h_Ecriture_Document_XML -> WriteEndDocument();

  } // FIN du try
  finally {
    //Obtention de l'état de l'écriture du document XML

    if (h_Ecriture_Document_XML -> WriteState != WriteState::Closed) {
      //Fin de l'écriture de données dans le document XML et fermeture du fichier
      h_Ecriture_Document_XML -> Close();
    }
  } //FIN du finally

  // Provisoire

  system("pause");
  ///////////////////

  return 0;
}