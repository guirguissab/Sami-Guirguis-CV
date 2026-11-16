SET search_path TO batnav_projet;


INSERT INTO joueur (id_J,Pseudo)
select idj, pseudo
from batnav.joueur;

INSERT INTO joueur_humain (id_J,Nom,Prenom,Date_Naiss,Date_Creation_Comte)
SELECT idj, nom, prenom, date_n, date_n::date
From batnav.joueur
  Where typeJ like 'Humain';

insert into joueur_virtuelle(id_J,Niveau,id_J_Createur,Date_Creation)
SELECT idj,niveau_exp_jv,idj_createur,date_c
from batnav.info_joueur_virtuel;

INSERT INTO Distribution_Carte (id_D, nom, c_missile, c_rejoue, c_vide, c_mpm, c_leurre, c_willy, c_mega, c_etoile, c_passe, c_oups)
SELECT idD, nom, c_missile, c_rejoue, c_vide, c_mpm, c_leurre, c_willy, c_mega, c_etoile, c_passe, c_oups
FROM batnav.distribution_cartes;

INSERT INTO pioche (id_Pi, id_D)
SELECT idPi, idDist
FROM batnav.Pioche;

INSERT INTO Type_Carte (Code_Carte, Bonus, Nom, Description, img)
SELECT
    codeTC,
    CASE
        WHEN est_bonus IS NULL THEN false
        ELSE est_bonus
    END,
    nom,
    description_tc,
    image_tc
FROM batnav.type_carte;

INSERT INTO Carte (id_Carte,Rang,id_Pi,Code_Carte)
SELECT idcarte,rang,idpi,codetc
FROM batnav.appartient;

-- peuplement du table partie est perdu mais c'etait peuple devant le pof

INSERT INTO Sequence_Temporelle (id_P, num_Seq, Date_Debut, Heure_Debut, Date_Fin, Heure_Fin)
SELECT
    idP, 1,
    seq_tempo_1_debut::date,
    seq_tempo_1_debut::time::text,
    seq_tempo_1_fin::date,
    seq_tempo_1_fin::time::text
FROM batnav.partie
WHERE seq_tempo_1_debut IS NOT NULL;

INSERT INTO Sequence_Temporelle (id_P, num_Seq, Date_Debut, Heure_Debut, Date_Fin, Heure_Fin)
SELECT
    idP, 2,
    seq_tempo_2_debut::date,
    seq_tempo_2_debut::time::text,
    seq_tempo_2_fin::date,
    seq_tempo_2_fin::time::text
FROM batnav.partie
WHERE seq_tempo_2_debut IS NOT NULL;

INSERT INTO Sequence_Temporelle (id_P, num_Seq, Date_Debut, Heure_Debut, Date_Fin, Heure_Fin)
SELECT
    idP, 3,
    seq_tempo_3_debut::date,
    seq_tempo_3_debut::time::text,
    seq_tempo_3_fin::date,
    seq_tempo_3_fin::time::text
FROM batnav.partie
WHERE seq_tempo_3_debut IS NOT NULL;


INSERT INTO tour(id_P,num_T,Nb_Navire_Coule_J,Nb_Cellule_Touche_J,Nb_Cellule_Exp_J
                ,Nb_Navire_Coule_A,Nb_Cellule_Touche_A,Nb_Cellule_Exp_A)
SELECT idp,numt,NULL,NULL,NULL,NULL,NULL,NULL
FROM batnav.tour
;

INSERT INTO Tir (id_P, num_T, num_Ti, Coord_X, Coord_Y, id_J, id_Carte)
SELECT
    idP,
    numT,
    numTi,
    coordx,
    coordy,
    idJ,
    idCarte
FROM batnav.tir;

INSERT INTO pavillion (code_Pays, nom_Pays) VALUES ('UK', 'Royaume-Uni');
INSERT INTO pavillion (code_Pays, nom_Pays) VALUES ('FR', 'France');
INSERT INTO pavillion (code_Pays, nom_Pays) VALUES ('US', 'Etats-Unis');
INSERT INTO pavillion (code_Pays, nom_Pays) VALUES ('CH', 'Chine');
INSERT INTO pavillion (code_Pays, nom_Pays) VALUES ('EG', 'Egypte');
INSERT INTO pavillion (code_Pays, nom_Pays) VALUES ('MAR', 'Maroc');

INSERT INTO Navire (nom, type_Navire, taille, code_Pays)
SELECT DISTINCT
nom_bat,
type_bat,
taille_bat,
(ARRAY['FR','UK','US','CH','EG','MAR'])[floor(random() * 6 + 1)::int]
FROM batnav.placements;


INSERT INTO Grille (id_P,Type_Grille, Largeur, Hauteur,
                    img_non_Exp, img_Vide, img_Touche)
SELECT DISTINCT
    p.idp,
    1,          
    10,
    10,
    'img/cellule_vide.png',
    'img/cellule_eau.png',
    'img/cellule_touche.png'
FROM batnav.placements p

UNION ALL

SELECT DISTINCT
    p.idp,
    2,          
    10,
    10,
    'img/cellule_vide.png',
    'img/cellule_eau.png',
    'img/cellule_touche.png'
FROM batnav.placements p;

INSERT INTO dispose_grille (id_J, id_G, id_P)
SELECT DISTINCT
    p.idj,
    g.id_g,
    p.idp
FROM batnav.placements p
JOIN grille g ON p.idp = g.id_p;




