--
-- PostgreSQL database dump
--


-- Dumped from database version 14.5 (Ubuntu 14.5-1.pgdg20.04+1)
-- Dumped by pg_dump version 16.13 (Ubuntu 16.13-0ubuntu0.24.04.1)

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- Name: batnav_projet; Type: SCHEMA; Schema: -; Owner: p2401496
--

CREATE SCHEMA batnav_projet;


ALTER SCHEMA batnav_projet OWNER TO p2401496;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- Name: appartient_flottille; Type: TABLE; Schema: batnav_projet; Owner: p2401496
--

CREATE TABLE batnav_projet.appartient_flottille (
    id_f integer NOT NULL,
    id_n integer NOT NULL
);


ALTER TABLE batnav_projet.appartient_flottille OWNER TO p2401496;

--
-- Name: appartient_flottille_id_f_seq; Type: SEQUENCE; Schema: batnav_projet; Owner: p2401496
--

CREATE SEQUENCE batnav_projet.appartient_flottille_id_f_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE batnav_projet.appartient_flottille_id_f_seq OWNER TO p2401496;

--
-- Name: appartient_flottille_id_f_seq; Type: SEQUENCE OWNED BY; Schema: batnav_projet; Owner: p2401496
--

ALTER SEQUENCE batnav_projet.appartient_flottille_id_f_seq OWNED BY batnav_projet.appartient_flottille.id_f;


--
-- Name: appartient_flottille_id_n_seq; Type: SEQUENCE; Schema: batnav_projet; Owner: p2401496
--

CREATE SEQUENCE batnav_projet.appartient_flottille_id_n_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE batnav_projet.appartient_flottille_id_n_seq OWNER TO p2401496;

--
-- Name: appartient_flottille_id_n_seq; Type: SEQUENCE OWNED BY; Schema: batnav_projet; Owner: p2401496
--

ALTER SEQUENCE batnav_projet.appartient_flottille_id_n_seq OWNED BY batnav_projet.appartient_flottille.id_n;


--
-- Name: carte; Type: TABLE; Schema: batnav_projet; Owner: p2401496
--

CREATE TABLE batnav_projet.carte (
    id_carte integer NOT NULL,
    rang integer,
    id_pi integer NOT NULL,
    code_carte text NOT NULL
);


ALTER TABLE batnav_projet.carte OWNER TO p2401496;

--
-- Name: carte_id_carte_seq; Type: SEQUENCE; Schema: batnav_projet; Owner: p2401496
--

CREATE SEQUENCE batnav_projet.carte_id_carte_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE batnav_projet.carte_id_carte_seq OWNER TO p2401496;

--
-- Name: carte_id_carte_seq; Type: SEQUENCE OWNED BY; Schema: batnav_projet; Owner: p2401496
--

ALTER SEQUENCE batnav_projet.carte_id_carte_seq OWNED BY batnav_projet.carte.id_carte;


--
-- Name: cellule_devoile; Type: TABLE; Schema: batnav_projet; Owner: p2401496
--

CREATE TABLE batnav_projet.cellule_devoile (
    id_p integer NOT NULL,
    num_t integer NOT NULL,
    num_ti integer NOT NULL,
    id_g integer NOT NULL,
    coord_x integer NOT NULL,
    coord_y character(1) NOT NULL
);


ALTER TABLE batnav_projet.cellule_devoile OWNER TO p2401496;

--
-- Name: cellule_grille; Type: TABLE; Schema: batnav_projet; Owner: p2401496
--

CREATE TABLE batnav_projet.cellule_grille (
    id_g integer NOT NULL,
    coord_x integer NOT NULL,
    coord_y character(1) NOT NULL,
    etat character varying(42),
    num_case text,
    id_n integer NOT NULL
);


ALTER TABLE batnav_projet.cellule_grille OWNER TO p2401496;

--
-- Name: cellule_grille_id_n_seq; Type: SEQUENCE; Schema: batnav_projet; Owner: p2401496
--

CREATE SEQUENCE batnav_projet.cellule_grille_id_n_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE batnav_projet.cellule_grille_id_n_seq OWNER TO p2401496;

--
-- Name: cellule_grille_id_n_seq; Type: SEQUENCE OWNED BY; Schema: batnav_projet; Owner: p2401496
--

ALTER SEQUENCE batnav_projet.cellule_grille_id_n_seq OWNED BY batnav_projet.cellule_grille.id_n;


--
-- Name: classement; Type: TABLE; Schema: batnav_projet; Owner: p2401496
--

CREATE TABLE batnav_projet.classement (
    id_c integer NOT NULL,
    delta_mois integer,
    type_classement text,
    date_calcul timestamp without time zone
);


ALTER TABLE batnav_projet.classement OWNER TO p2401496;

--
-- Name: classement_cpp; Type: TABLE; Schema: batnav_projet; Owner: p2401496
--

CREATE TABLE batnav_projet.classement_cpp (
    id_c integer NOT NULL,
    code_pays text NOT NULL,
    score_total integer,
    rang integer
);


ALTER TABLE batnav_projet.classement_cpp OWNER TO p2401496;

--
-- Name: classement_ijh; Type: TABLE; Schema: batnav_projet; Owner: p2401496
--

CREATE TABLE batnav_projet.classement_ijh (
    id_j integer NOT NULL,
    rang integer NOT NULL,
    score_total integer,
    id_c integer NOT NULL
);


ALTER TABLE batnav_projet.classement_ijh OWNER TO p2401496;

--
-- Name: classement_ijh_id_j_seq; Type: SEQUENCE; Schema: batnav_projet; Owner: p2401496
--

CREATE SEQUENCE batnav_projet.classement_ijh_id_j_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE batnav_projet.classement_ijh_id_j_seq OWNER TO p2401496;

--
-- Name: classement_ijh_id_j_seq; Type: SEQUENCE OWNED BY; Schema: batnav_projet; Owner: p2401496
--

ALTER SEQUENCE batnav_projet.classement_ijh_id_j_seq OWNED BY batnav_projet.classement_ijh.id_j;


--
-- Name: dispose_grille; Type: TABLE; Schema: batnav_projet; Owner: p2401496
--

CREATE TABLE batnav_projet.dispose_grille (
    id_j integer NOT NULL,
    id_g integer NOT NULL,
    id_p integer NOT NULL
);


ALTER TABLE batnav_projet.dispose_grille OWNER TO p2401496;

--
-- Name: dispose_grille_id_g_seq; Type: SEQUENCE; Schema: batnav_projet; Owner: p2401496
--

CREATE SEQUENCE batnav_projet.dispose_grille_id_g_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE batnav_projet.dispose_grille_id_g_seq OWNER TO p2401496;

--
-- Name: dispose_grille_id_g_seq; Type: SEQUENCE OWNED BY; Schema: batnav_projet; Owner: p2401496
--

ALTER SEQUENCE batnav_projet.dispose_grille_id_g_seq OWNED BY batnav_projet.dispose_grille.id_g;


--
-- Name: dispose_grille_id_j_seq; Type: SEQUENCE; Schema: batnav_projet; Owner: p2401496
--

CREATE SEQUENCE batnav_projet.dispose_grille_id_j_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE batnav_projet.dispose_grille_id_j_seq OWNER TO p2401496;

--
-- Name: dispose_grille_id_j_seq; Type: SEQUENCE OWNED BY; Schema: batnav_projet; Owner: p2401496
--

ALTER SEQUENCE batnav_projet.dispose_grille_id_j_seq OWNED BY batnav_projet.dispose_grille.id_j;


--
-- Name: distribution_carte; Type: TABLE; Schema: batnav_projet; Owner: p2401496
--

CREATE TABLE batnav_projet.distribution_carte (
    id_d integer NOT NULL,
    nom text,
    c_missile integer,
    c_rejoue integer,
    c_vide integer,
    c_mpm integer,
    c_leurre integer,
    c_willy integer,
    c_mega integer,
    c_etoile integer,
    c_passe integer,
    c_oups integer
);


ALTER TABLE batnav_projet.distribution_carte OWNER TO p2401496;

--
-- Name: distribution_carte_id_d_seq; Type: SEQUENCE; Schema: batnav_projet; Owner: p2401496
--

CREATE SEQUENCE batnav_projet.distribution_carte_id_d_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE batnav_projet.distribution_carte_id_d_seq OWNER TO p2401496;

--
-- Name: distribution_carte_id_d_seq; Type: SEQUENCE OWNED BY; Schema: batnav_projet; Owner: p2401496
--

ALTER SEQUENCE batnav_projet.distribution_carte_id_d_seq OWNED BY batnav_projet.distribution_carte.id_d;


--
-- Name: est_place; Type: TABLE; Schema: batnav_projet; Owner: p2401496
--

CREATE TABLE batnav_projet.est_place (
    id_p integer NOT NULL,
    id_g integer NOT NULL,
    id_n integer NOT NULL,
    coord_x integer,
    coord_y character(1),
    sens text,
    etat text
);


ALTER TABLE batnav_projet.est_place OWNER TO p2401496;

--
-- Name: est_place_id_n_seq; Type: SEQUENCE; Schema: batnav_projet; Owner: p2401496
--

CREATE SEQUENCE batnav_projet.est_place_id_n_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE batnav_projet.est_place_id_n_seq OWNER TO p2401496;

--
-- Name: est_place_id_n_seq; Type: SEQUENCE OWNED BY; Schema: batnav_projet; Owner: p2401496
--

ALTER SEQUENCE batnav_projet.est_place_id_n_seq OWNED BY batnav_projet.est_place.id_n;


--
-- Name: flottille; Type: TABLE; Schema: batnav_projet; Owner: p2401496
--

CREATE TABLE batnav_projet.flottille (
    id_f integer NOT NULL,
    type_flottille text,
    nb_navires integer
);


ALTER TABLE batnav_projet.flottille OWNER TO p2401496;

--
-- Name: flottille_id_f_seq; Type: SEQUENCE; Schema: batnav_projet; Owner: p2401496
--

CREATE SEQUENCE batnav_projet.flottille_id_f_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE batnav_projet.flottille_id_f_seq OWNER TO p2401496;

--
-- Name: flottille_id_f_seq; Type: SEQUENCE OWNED BY; Schema: batnav_projet; Owner: p2401496
--

ALTER SEQUENCE batnav_projet.flottille_id_f_seq OWNED BY batnav_projet.flottille.id_f;


--
-- Name: grille; Type: TABLE; Schema: batnav_projet; Owner: p2401496
--

CREATE TABLE batnav_projet.grille (
    id_g integer NOT NULL,
    id_p integer NOT NULL,
    type_grille integer,
    largeur integer,
    hauteur integer,
    img_non_exp text,
    img_vide text,
    img_touche text
);


ALTER TABLE batnav_projet.grille OWNER TO p2401496;

--
-- Name: grille_id_g_seq; Type: SEQUENCE; Schema: batnav_projet; Owner: p2401496
--

CREATE SEQUENCE batnav_projet.grille_id_g_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE batnav_projet.grille_id_g_seq OWNER TO p2401496;

--
-- Name: grille_id_g_seq; Type: SEQUENCE OWNED BY; Schema: batnav_projet; Owner: p2401496
--

ALTER SEQUENCE batnav_projet.grille_id_g_seq OWNED BY batnav_projet.grille.id_g;


--
-- Name: joueur; Type: TABLE; Schema: batnav_projet; Owner: p2401496
--

CREATE TABLE batnav_projet.joueur (
    id_j integer NOT NULL,
    pseudo text
);


ALTER TABLE batnav_projet.joueur OWNER TO p2401496;

--
-- Name: joueur_humain; Type: TABLE; Schema: batnav_projet; Owner: p2401496
--

CREATE TABLE batnav_projet.joueur_humain (
    id_j integer NOT NULL,
    nom text,
    prenom text,
    date_naiss date,
    date_creation_comte date
);


ALTER TABLE batnav_projet.joueur_humain OWNER TO p2401496;

--
-- Name: joueur_humain_id_j_seq; Type: SEQUENCE; Schema: batnav_projet; Owner: p2401496
--

CREATE SEQUENCE batnav_projet.joueur_humain_id_j_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE batnav_projet.joueur_humain_id_j_seq OWNER TO p2401496;

--
-- Name: joueur_humain_id_j_seq; Type: SEQUENCE OWNED BY; Schema: batnav_projet; Owner: p2401496
--

ALTER SEQUENCE batnav_projet.joueur_humain_id_j_seq OWNED BY batnav_projet.joueur_humain.id_j;


--
-- Name: joueur_id_j_seq; Type: SEQUENCE; Schema: batnav_projet; Owner: p2401496
--

CREATE SEQUENCE batnav_projet.joueur_id_j_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE batnav_projet.joueur_id_j_seq OWNER TO p2401496;

--
-- Name: joueur_id_j_seq; Type: SEQUENCE OWNED BY; Schema: batnav_projet; Owner: p2401496
--

ALTER SEQUENCE batnav_projet.joueur_id_j_seq OWNED BY batnav_projet.joueur.id_j;


--
-- Name: joueur_virtuelle; Type: TABLE; Schema: batnav_projet; Owner: p2401496
--

CREATE TABLE batnav_projet.joueur_virtuelle (
    id_j integer NOT NULL,
    niveau text,
    id_j_createur integer NOT NULL,
    date_creation date
);


ALTER TABLE batnav_projet.joueur_virtuelle OWNER TO p2401496;

--
-- Name: joueur_virtuelle_id_j_createur_seq; Type: SEQUENCE; Schema: batnav_projet; Owner: p2401496
--

CREATE SEQUENCE batnav_projet.joueur_virtuelle_id_j_createur_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE batnav_projet.joueur_virtuelle_id_j_createur_seq OWNER TO p2401496;

--
-- Name: joueur_virtuelle_id_j_createur_seq; Type: SEQUENCE OWNED BY; Schema: batnav_projet; Owner: p2401496
--

ALTER SEQUENCE batnav_projet.joueur_virtuelle_id_j_createur_seq OWNED BY batnav_projet.joueur_virtuelle.id_j_createur;


--
-- Name: joueur_virtuelle_id_j_seq; Type: SEQUENCE; Schema: batnav_projet; Owner: p2401496
--

CREATE SEQUENCE batnav_projet.joueur_virtuelle_id_j_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE batnav_projet.joueur_virtuelle_id_j_seq OWNER TO p2401496;

--
-- Name: joueur_virtuelle_id_j_seq; Type: SEQUENCE OWNED BY; Schema: batnav_projet; Owner: p2401496
--

ALTER SEQUENCE batnav_projet.joueur_virtuelle_id_j_seq OWNED BY batnav_projet.joueur_virtuelle.id_j;


--
-- Name: leurre; Type: TABLE; Schema: batnav_projet; Owner: p2401496
--

CREATE TABLE batnav_projet.leurre (
    id_g integer NOT NULL,
    num_l integer NOT NULL,
    taille integer,
    coord_x integer,
    coord_y character(1),
    sens text,
    etat text
);


ALTER TABLE batnav_projet.leurre OWNER TO p2401496;

--
-- Name: navire; Type: TABLE; Schema: batnav_projet; Owner: p2401496
--

CREATE TABLE batnav_projet.navire (
    id_n integer NOT NULL,
    nom text,
    type_navire text,
    taille integer,
    code_pays text NOT NULL
);


ALTER TABLE batnav_projet.navire OWNER TO p2401496;

--
-- Name: navire_id_n_seq; Type: SEQUENCE; Schema: batnav_projet; Owner: p2401496
--

CREATE SEQUENCE batnav_projet.navire_id_n_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE batnav_projet.navire_id_n_seq OWNER TO p2401496;

--
-- Name: navire_id_n_seq; Type: SEQUENCE OWNED BY; Schema: batnav_projet; Owner: p2401496
--

ALTER SEQUENCE batnav_projet.navire_id_n_seq OWNED BY batnav_projet.navire.id_n;


--
-- Name: orque; Type: TABLE; Schema: batnav_projet; Owner: p2401496
--

CREATE TABLE batnav_projet.orque (
    id_q integer NOT NULL,
    nom text,
    img text
);


ALTER TABLE batnav_projet.orque OWNER TO p2401496;

--
-- Name: orque_id_q_seq; Type: SEQUENCE; Schema: batnav_projet; Owner: p2401496
--

CREATE SEQUENCE batnav_projet.orque_id_q_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE batnav_projet.orque_id_q_seq OWNER TO p2401496;

--
-- Name: orque_id_q_seq; Type: SEQUENCE OWNED BY; Schema: batnav_projet; Owner: p2401496
--

ALTER SEQUENCE batnav_projet.orque_id_q_seq OWNED BY batnav_projet.orque.id_q;


--
-- Name: partie; Type: TABLE; Schema: batnav_projet; Owner: p2401496
--

CREATE TABLE batnav_projet.partie (
    id_p integer NOT NULL,
    date_creation date,
    heure_creation text,
    etat text,
    score_final integer,
    id_j_humain integer NOT NULL,
    id_j_virtuelle integer NOT NULL,
    id_pi integer NOT NULL,
    id_j_gagnant integer
);


ALTER TABLE batnav_projet.partie OWNER TO p2401496;

--
-- Name: partie_id_j_humain_seq; Type: SEQUENCE; Schema: batnav_projet; Owner: p2401496
--

CREATE SEQUENCE batnav_projet.partie_id_j_humain_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE batnav_projet.partie_id_j_humain_seq OWNER TO p2401496;

--
-- Name: partie_id_j_humain_seq; Type: SEQUENCE OWNED BY; Schema: batnav_projet; Owner: p2401496
--

ALTER SEQUENCE batnav_projet.partie_id_j_humain_seq OWNED BY batnav_projet.partie.id_j_humain;


--
-- Name: partie_id_j_virtuelle_seq; Type: SEQUENCE; Schema: batnav_projet; Owner: p2401496
--

CREATE SEQUENCE batnav_projet.partie_id_j_virtuelle_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE batnav_projet.partie_id_j_virtuelle_seq OWNER TO p2401496;

--
-- Name: partie_id_j_virtuelle_seq; Type: SEQUENCE OWNED BY; Schema: batnav_projet; Owner: p2401496
--

ALTER SEQUENCE batnav_projet.partie_id_j_virtuelle_seq OWNED BY batnav_projet.partie.id_j_virtuelle;


--
-- Name: pavillion; Type: TABLE; Schema: batnav_projet; Owner: p2401496
--

CREATE TABLE batnav_projet.pavillion (
    code_pays text NOT NULL,
    nom_pays text
);


ALTER TABLE batnav_projet.pavillion OWNER TO p2401496;

--
-- Name: peut_contenir; Type: TABLE; Schema: batnav_projet; Owner: p2401496
--

CREATE TABLE batnav_projet.peut_contenir (
    id_g integer NOT NULL,
    id_q integer NOT NULL,
    coord_x integer,
    coord_y character(1)
);


ALTER TABLE batnav_projet.peut_contenir OWNER TO p2401496;

--
-- Name: peut_contenir_id_q_seq; Type: SEQUENCE; Schema: batnav_projet; Owner: p2401496
--

CREATE SEQUENCE batnav_projet.peut_contenir_id_q_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE batnav_projet.peut_contenir_id_q_seq OWNER TO p2401496;

--
-- Name: peut_contenir_id_q_seq; Type: SEQUENCE OWNED BY; Schema: batnav_projet; Owner: p2401496
--

ALTER SEQUENCE batnav_projet.peut_contenir_id_q_seq OWNED BY batnav_projet.peut_contenir.id_q;


--
-- Name: pioche; Type: TABLE; Schema: batnav_projet; Owner: p2401496
--

CREATE TABLE batnav_projet.pioche (
    id_pi integer NOT NULL,
    id_d integer NOT NULL
);


ALTER TABLE batnav_projet.pioche OWNER TO p2401496;

--
-- Name: pioche_id_d_seq; Type: SEQUENCE; Schema: batnav_projet; Owner: p2401496
--

CREATE SEQUENCE batnav_projet.pioche_id_d_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE batnav_projet.pioche_id_d_seq OWNER TO p2401496;

--
-- Name: pioche_id_d_seq; Type: SEQUENCE OWNED BY; Schema: batnav_projet; Owner: p2401496
--

ALTER SEQUENCE batnav_projet.pioche_id_d_seq OWNED BY batnav_projet.pioche.id_d;


--
-- Name: sequence_temporelle; Type: TABLE; Schema: batnav_projet; Owner: p2401496
--

CREATE TABLE batnav_projet.sequence_temporelle (
    id_p integer NOT NULL,
    num_seq integer NOT NULL,
    date_debut date,
    heure_debut text,
    date_fin date,
    heure_fin text
);


ALTER TABLE batnav_projet.sequence_temporelle OWNER TO p2401496;

--
-- Name: tir; Type: TABLE; Schema: batnav_projet; Owner: p2401496
--

CREATE TABLE batnav_projet.tir (
    id_p integer NOT NULL,
    num_t integer NOT NULL,
    num_ti integer NOT NULL,
    coord_x integer,
    coord_y character(1),
    id_j integer NOT NULL,
    id_carte integer NOT NULL
);


ALTER TABLE batnav_projet.tir OWNER TO p2401496;

--
-- Name: tir_id_carte_seq; Type: SEQUENCE; Schema: batnav_projet; Owner: p2401496
--

CREATE SEQUENCE batnav_projet.tir_id_carte_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE batnav_projet.tir_id_carte_seq OWNER TO p2401496;

--
-- Name: tir_id_carte_seq; Type: SEQUENCE OWNED BY; Schema: batnav_projet; Owner: p2401496
--

ALTER SEQUENCE batnav_projet.tir_id_carte_seq OWNED BY batnav_projet.tir.id_carte;


--
-- Name: tir_id_j_seq; Type: SEQUENCE; Schema: batnav_projet; Owner: p2401496
--

CREATE SEQUENCE batnav_projet.tir_id_j_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE batnav_projet.tir_id_j_seq OWNER TO p2401496;

--
-- Name: tir_id_j_seq; Type: SEQUENCE OWNED BY; Schema: batnav_projet; Owner: p2401496
--

ALTER SEQUENCE batnav_projet.tir_id_j_seq OWNED BY batnav_projet.tir.id_j;


--
-- Name: tour; Type: TABLE; Schema: batnav_projet; Owner: p2401496
--

CREATE TABLE batnav_projet.tour (
    id_p integer NOT NULL,
    num_t integer NOT NULL,
    nb_navire_coule_j integer,
    nb_cellule_touche_j integer,
    nb_cellule_exp_j integer,
    nb_navire_coule_a integer,
    nb_cellule_touche_a integer,
    nb_cellule_exp_a integer
);


ALTER TABLE batnav_projet.tour OWNER TO p2401496;

--
-- Name: type_carte; Type: TABLE; Schema: batnav_projet; Owner: p2401496
--

CREATE TABLE batnav_projet.type_carte (
    code_carte text NOT NULL,
    bonus boolean,
    nom text,
    description text,
    img text
);


ALTER TABLE batnav_projet.type_carte OWNER TO p2401496;

--
-- Name: appartient_flottille id_f; Type: DEFAULT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.appartient_flottille ALTER COLUMN id_f SET DEFAULT nextval('batnav_projet.appartient_flottille_id_f_seq'::regclass);


--
-- Name: appartient_flottille id_n; Type: DEFAULT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.appartient_flottille ALTER COLUMN id_n SET DEFAULT nextval('batnav_projet.appartient_flottille_id_n_seq'::regclass);


--
-- Name: carte id_carte; Type: DEFAULT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.carte ALTER COLUMN id_carte SET DEFAULT nextval('batnav_projet.carte_id_carte_seq'::regclass);


--
-- Name: cellule_grille id_n; Type: DEFAULT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.cellule_grille ALTER COLUMN id_n SET DEFAULT nextval('batnav_projet.cellule_grille_id_n_seq'::regclass);


--
-- Name: classement_ijh id_j; Type: DEFAULT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.classement_ijh ALTER COLUMN id_j SET DEFAULT nextval('batnav_projet.classement_ijh_id_j_seq'::regclass);


--
-- Name: dispose_grille id_j; Type: DEFAULT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.dispose_grille ALTER COLUMN id_j SET DEFAULT nextval('batnav_projet.dispose_grille_id_j_seq'::regclass);


--
-- Name: dispose_grille id_g; Type: DEFAULT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.dispose_grille ALTER COLUMN id_g SET DEFAULT nextval('batnav_projet.dispose_grille_id_g_seq'::regclass);


--
-- Name: distribution_carte id_d; Type: DEFAULT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.distribution_carte ALTER COLUMN id_d SET DEFAULT nextval('batnav_projet.distribution_carte_id_d_seq'::regclass);


--
-- Name: est_place id_n; Type: DEFAULT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.est_place ALTER COLUMN id_n SET DEFAULT nextval('batnav_projet.est_place_id_n_seq'::regclass);


--
-- Name: flottille id_f; Type: DEFAULT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.flottille ALTER COLUMN id_f SET DEFAULT nextval('batnav_projet.flottille_id_f_seq'::regclass);


--
-- Name: grille id_g; Type: DEFAULT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.grille ALTER COLUMN id_g SET DEFAULT nextval('batnav_projet.grille_id_g_seq'::regclass);


--
-- Name: joueur id_j; Type: DEFAULT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.joueur ALTER COLUMN id_j SET DEFAULT nextval('batnav_projet.joueur_id_j_seq'::regclass);


--
-- Name: joueur_humain id_j; Type: DEFAULT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.joueur_humain ALTER COLUMN id_j SET DEFAULT nextval('batnav_projet.joueur_humain_id_j_seq'::regclass);


--
-- Name: joueur_virtuelle id_j; Type: DEFAULT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.joueur_virtuelle ALTER COLUMN id_j SET DEFAULT nextval('batnav_projet.joueur_virtuelle_id_j_seq'::regclass);


--
-- Name: joueur_virtuelle id_j_createur; Type: DEFAULT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.joueur_virtuelle ALTER COLUMN id_j_createur SET DEFAULT nextval('batnav_projet.joueur_virtuelle_id_j_createur_seq'::regclass);


--
-- Name: navire id_n; Type: DEFAULT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.navire ALTER COLUMN id_n SET DEFAULT nextval('batnav_projet.navire_id_n_seq'::regclass);


--
-- Name: orque id_q; Type: DEFAULT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.orque ALTER COLUMN id_q SET DEFAULT nextval('batnav_projet.orque_id_q_seq'::regclass);


--
-- Name: partie id_j_humain; Type: DEFAULT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.partie ALTER COLUMN id_j_humain SET DEFAULT nextval('batnav_projet.partie_id_j_humain_seq'::regclass);


--
-- Name: partie id_j_virtuelle; Type: DEFAULT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.partie ALTER COLUMN id_j_virtuelle SET DEFAULT nextval('batnav_projet.partie_id_j_virtuelle_seq'::regclass);


--
-- Name: peut_contenir id_q; Type: DEFAULT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.peut_contenir ALTER COLUMN id_q SET DEFAULT nextval('batnav_projet.peut_contenir_id_q_seq'::regclass);


--
-- Name: pioche id_d; Type: DEFAULT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.pioche ALTER COLUMN id_d SET DEFAULT nextval('batnav_projet.pioche_id_d_seq'::regclass);


--
-- Name: tir id_j; Type: DEFAULT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.tir ALTER COLUMN id_j SET DEFAULT nextval('batnav_projet.tir_id_j_seq'::regclass);


--
-- Name: tir id_carte; Type: DEFAULT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.tir ALTER COLUMN id_carte SET DEFAULT nextval('batnav_projet.tir_id_carte_seq'::regclass);


--
-- Data for Name: appartient_flottille; Type: TABLE DATA; Schema: batnav_projet; Owner: p2401496
--



--
-- Data for Name: carte; Type: TABLE DATA; Schema: batnav_projet; Owner: p2401496
--

INSERT INTO batnav_projet.carte VALUES (1, 1, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (2, 2, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (3, 3, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (4, 4, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (5, 5, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (6, 6, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (7, 7, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (8, 8, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (9, 9, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (10, 10, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (11, 11, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (12, 12, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (13, 13, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (14, 14, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (15, 15, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (16, 16, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (17, 17, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (18, 18, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (19, 19, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (20, 20, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (21, 21, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (22, 22, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (23, 23, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (24, 24, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (25, 25, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (26, 26, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (27, 27, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (28, 28, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (29, 29, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (30, 30, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (31, 31, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (32, 32, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (33, 33, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (34, 34, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (35, 35, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (36, 36, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (37, 37, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (38, 38, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (39, 39, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (40, 40, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (41, 41, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (42, 42, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (43, 43, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (44, 44, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (45, 45, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (46, 46, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (47, 47, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (48, 48, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (49, 49, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (50, 50, 105, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (51, 51, 105, 'C_REJOUE');
INSERT INTO batnav_projet.carte VALUES (52, 52, 105, 'C_REJOUE');
INSERT INTO batnav_projet.carte VALUES (53, 53, 105, 'C_REJOUE');
INSERT INTO batnav_projet.carte VALUES (54, 54, 105, 'C_REJOUE');
INSERT INTO batnav_projet.carte VALUES (55, 55, 105, 'C_REJOUE');
INSERT INTO batnav_projet.carte VALUES (56, 56, 105, 'C_REJOUE');
INSERT INTO batnav_projet.carte VALUES (57, 57, 105, 'C_REJOUE');
INSERT INTO batnav_projet.carte VALUES (58, 58, 105, 'C_REJOUE');
INSERT INTO batnav_projet.carte VALUES (59, 59, 105, 'C_REJOUE');
INSERT INTO batnav_projet.carte VALUES (60, 60, 105, 'C_REJOUE');
INSERT INTO batnav_projet.carte VALUES (61, 61, 105, 'C_VIDE');
INSERT INTO batnav_projet.carte VALUES (62, 62, 105, 'C_VIDE');
INSERT INTO batnav_projet.carte VALUES (63, 63, 105, 'C_VIDE');
INSERT INTO batnav_projet.carte VALUES (64, 64, 105, 'C_VIDE');
INSERT INTO batnav_projet.carte VALUES (65, 65, 105, 'C_VIDE');
INSERT INTO batnav_projet.carte VALUES (66, 66, 105, 'C_VIDE');
INSERT INTO batnav_projet.carte VALUES (67, 67, 105, 'C_VIDE');
INSERT INTO batnav_projet.carte VALUES (68, 68, 105, 'C_VIDE');
INSERT INTO batnav_projet.carte VALUES (69, 69, 105, 'C_VIDE');
INSERT INTO batnav_projet.carte VALUES (70, 70, 105, 'C_VIDE');
INSERT INTO batnav_projet.carte VALUES (71, 71, 105, 'C_MPM');
INSERT INTO batnav_projet.carte VALUES (72, 72, 105, 'C_MPM');
INSERT INTO batnav_projet.carte VALUES (73, 73, 105, 'C_MPM');
INSERT INTO batnav_projet.carte VALUES (74, 74, 105, 'C_MPM');
INSERT INTO batnav_projet.carte VALUES (75, 75, 105, 'C_MPM');
INSERT INTO batnav_projet.carte VALUES (76, 76, 105, 'C_LEURRE');
INSERT INTO batnav_projet.carte VALUES (77, 77, 105, 'C_LEURRE');
INSERT INTO batnav_projet.carte VALUES (78, 78, 105, 'C_LEURRE');
INSERT INTO batnav_projet.carte VALUES (79, 79, 105, 'C_WILLY');
INSERT INTO batnav_projet.carte VALUES (80, 80, 105, 'C_WILLY');
INSERT INTO batnav_projet.carte VALUES (81, 81, 105, 'C_WILLY');
INSERT INTO batnav_projet.carte VALUES (82, 82, 105, 'C_MEGA');
INSERT INTO batnav_projet.carte VALUES (83, 83, 105, 'C_MEGA');
INSERT INTO batnav_projet.carte VALUES (84, 84, 105, 'C_MEGA');
INSERT INTO batnav_projet.carte VALUES (85, 85, 105, 'C_ETOILE');
INSERT INTO batnav_projet.carte VALUES (86, 86, 105, 'C_PASSE');
INSERT INTO batnav_projet.carte VALUES (87, 87, 105, 'C_PASSE');
INSERT INTO batnav_projet.carte VALUES (88, 88, 105, 'C_PASSE');
INSERT INTO batnav_projet.carte VALUES (89, 89, 105, 'C_PASSE');
INSERT INTO batnav_projet.carte VALUES (90, 90, 105, 'C_PASSE');
INSERT INTO batnav_projet.carte VALUES (91, 91, 105, 'C_PASSE');
INSERT INTO batnav_projet.carte VALUES (92, 92, 105, 'C_PASSE');
INSERT INTO batnav_projet.carte VALUES (93, 93, 105, 'C_PASSE');
INSERT INTO batnav_projet.carte VALUES (94, 94, 105, 'C_PASSE');
INSERT INTO batnav_projet.carte VALUES (95, 95, 105, 'C_PASSE');
INSERT INTO batnav_projet.carte VALUES (96, 96, 105, 'C_OUPS');
INSERT INTO batnav_projet.carte VALUES (97, 97, 105, 'C_OUPS');
INSERT INTO batnav_projet.carte VALUES (98, 98, 105, 'C_OUPS');
INSERT INTO batnav_projet.carte VALUES (99, 99, 105, 'C_OUPS');
INSERT INTO batnav_projet.carte VALUES (100, 100, 105, 'C_OUPS');
INSERT INTO batnav_projet.carte VALUES (101, 1, 103, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (102, 2, 103, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (103, 3, 103, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (104, 4, 103, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (105, 5, 103, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (106, 6, 103, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (107, 7, 103, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (108, 8, 103, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (109, 9, 103, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (110, 10, 103, 'C_MISSILE');
INSERT INTO batnav_projet.carte VALUES (111, 11, 103, 'C_REJOUE');
INSERT INTO batnav_projet.carte VALUES (112, 12, 103, 'C_REJOUE');
INSERT INTO batnav_projet.carte VALUES (113, 13, 103, 'C_REJOUE');
INSERT INTO batnav_projet.carte VALUES (114, 14, 103, 'C_REJOUE');
INSERT INTO batnav_projet.carte VALUES (115, 15, 103, 'C_REJOUE');
INSERT INTO batnav_projet.carte VALUES (116, 16, 103, 'C_VIDE');
INSERT INTO batnav_projet.carte VALUES (117, 17, 103, 'C_VIDE');
INSERT INTO batnav_projet.carte VALUES (118, 18, 103, 'C_VIDE');
INSERT INTO batnav_projet.carte VALUES (119, 19, 103, 'C_VIDE');
INSERT INTO batnav_projet.carte VALUES (120, 20, 103, 'C_VIDE');
INSERT INTO batnav_projet.carte VALUES (121, 21, 103, 'C_VIDE');
INSERT INTO batnav_projet.carte VALUES (122, 22, 103, 'C_VIDE');
INSERT INTO batnav_projet.carte VALUES (123, 23, 103, 'C_VIDE');
INSERT INTO batnav_projet.carte VALUES (124, 24, 103, 'C_VIDE');
INSERT INTO batnav_projet.carte VALUES (125, 25, 103, 'C_VIDE');
INSERT INTO batnav_projet.carte VALUES (126, 26, 103, 'C_VIDE');
INSERT INTO batnav_projet.carte VALUES (127, 27, 103, 'C_VIDE');
INSERT INTO batnav_projet.carte VALUES (128, 28, 103, 'C_VIDE');
INSERT INTO batnav_projet.carte VALUES (129, 29, 103, 'C_VIDE');
INSERT INTO batnav_projet.carte VALUES (130, 30, 103, 'C_VIDE');
INSERT INTO batnav_projet.carte VALUES (131, 31, 103, 'C_MPM');
INSERT INTO batnav_projet.carte VALUES (132, 32, 103, 'C_MPM');
INSERT INTO batnav_projet.carte VALUES (133, 33, 103, 'C_MPM');
INSERT INTO batnav_projet.carte VALUES (134, 34, 103, 'C_MPM');
INSERT INTO batnav_projet.carte VALUES (135, 35, 103, 'C_MPM');
INSERT INTO batnav_projet.carte VALUES (136, 36, 103, 'C_MPM');
INSERT INTO batnav_projet.carte VALUES (137, 37, 103, 'C_MPM');
INSERT INTO batnav_projet.carte VALUES (138, 38, 103, 'C_MPM');
INSERT INTO batnav_projet.carte VALUES (139, 39, 103, 'C_MPM');
INSERT INTO batnav_projet.carte VALUES (140, 40, 103, 'C_MPM');
INSERT INTO batnav_projet.carte VALUES (141, 41, 103, 'C_LEURRE');
INSERT INTO batnav_projet.carte VALUES (142, 42, 103, 'C_LEURRE');
INSERT INTO batnav_projet.carte VALUES (143, 43, 103, 'C_LEURRE');
INSERT INTO batnav_projet.carte VALUES (144, 44, 103, 'C_LEURRE');
INSERT INTO batnav_projet.carte VALUES (145, 45, 103, 'C_LEURRE');
INSERT INTO batnav_projet.carte VALUES (146, 46, 103, 'C_LEURRE');
INSERT INTO batnav_projet.carte VALUES (147, 47, 103, 'C_LEURRE');
INSERT INTO batnav_projet.carte VALUES (148, 48, 103, 'C_LEURRE');
INSERT INTO batnav_projet.carte VALUES (149, 49, 103, 'C_LEURRE');
INSERT INTO batnav_projet.carte VALUES (150, 50, 103, 'C_LEURRE');
INSERT INTO batnav_projet.carte VALUES (151, 51, 103, 'C_WILLY');
INSERT INTO batnav_projet.carte VALUES (152, 52, 103, 'C_WILLY');
INSERT INTO batnav_projet.carte VALUES (153, 53, 103, 'C_WILLY');
INSERT INTO batnav_projet.carte VALUES (154, 54, 103, 'C_WILLY');
INSERT INTO batnav_projet.carte VALUES (155, 55, 103, 'C_WILLY');
INSERT INTO batnav_projet.carte VALUES (156, 56, 103, 'C_WILLY');
INSERT INTO batnav_projet.carte VALUES (157, 57, 103, 'C_WILLY');
INSERT INTO batnav_projet.carte VALUES (158, 58, 103, 'C_WILLY');
INSERT INTO batnav_projet.carte VALUES (159, 59, 103, 'C_WILLY');
INSERT INTO batnav_projet.carte VALUES (160, 60, 103, 'C_WILLY');
INSERT INTO batnav_projet.carte VALUES (161, 61, 103, 'C_MEGA');
INSERT INTO batnav_projet.carte VALUES (162, 62, 103, 'C_MEGA');
INSERT INTO batnav_projet.carte VALUES (163, 63, 103, 'C_MEGA');
INSERT INTO batnav_projet.carte VALUES (164, 64, 103, 'C_MEGA');
INSERT INTO batnav_projet.carte VALUES (165, 65, 103, 'C_MEGA');
INSERT INTO batnav_projet.carte VALUES (166, 66, 103, 'C_MEGA');
INSERT INTO batnav_projet.carte VALUES (167, 67, 103, 'C_MEGA');
INSERT INTO batnav_projet.carte VALUES (168, 68, 103, 'C_MEGA');
INSERT INTO batnav_projet.carte VALUES (169, 69, 103, 'C_MEGA');
INSERT INTO batnav_projet.carte VALUES (170, 70, 103, 'C_MEGA');
INSERT INTO batnav_projet.carte VALUES (171, 71, 103, 'C_ETOILE');
INSERT INTO batnav_projet.carte VALUES (172, 72, 103, 'C_ETOILE');
INSERT INTO batnav_projet.carte VALUES (173, 73, 103, 'C_ETOILE');
INSERT INTO batnav_projet.carte VALUES (174, 74, 103, 'C_ETOILE');
INSERT INTO batnav_projet.carte VALUES (175, 75, 103, 'C_ETOILE');
INSERT INTO batnav_projet.carte VALUES (176, 76, 103, 'C_PASSE');
INSERT INTO batnav_projet.carte VALUES (177, 77, 103, 'C_PASSE');
INSERT INTO batnav_projet.carte VALUES (178, 78, 103, 'C_PASSE');
INSERT INTO batnav_projet.carte VALUES (179, 79, 103, 'C_PASSE');
INSERT INTO batnav_projet.carte VALUES (180, 80, 103, 'C_PASSE');
INSERT INTO batnav_projet.carte VALUES (181, 81, 103, 'C_OUPS');
INSERT INTO batnav_projet.carte VALUES (182, 82, 103, 'C_OUPS');
INSERT INTO batnav_projet.carte VALUES (183, 83, 103, 'C_OUPS');
INSERT INTO batnav_projet.carte VALUES (184, 84, 103, 'C_OUPS');
INSERT INTO batnav_projet.carte VALUES (185, 85, 103, 'C_OUPS');
INSERT INTO batnav_projet.carte VALUES (186, 86, 103, 'C_OUPS');
INSERT INTO batnav_projet.carte VALUES (187, 87, 103, 'C_OUPS');
INSERT INTO batnav_projet.carte VALUES (188, 88, 103, 'C_OUPS');
INSERT INTO batnav_projet.carte VALUES (189, 89, 103, 'C_OUPS');
INSERT INTO batnav_projet.carte VALUES (190, 90, 103, 'C_OUPS');
INSERT INTO batnav_projet.carte VALUES (191, 91, 103, 'C_OUPS');
INSERT INTO batnav_projet.carte VALUES (192, 92, 103, 'C_OUPS');
INSERT INTO batnav_projet.carte VALUES (193, 93, 103, 'C_OUPS');
INSERT INTO batnav_projet.carte VALUES (194, 94, 103, 'C_OUPS');
INSERT INTO batnav_projet.carte VALUES (195, 95, 103, 'C_OUPS');
INSERT INTO batnav_projet.carte VALUES (196, 96, 103, 'C_OUPS');
INSERT INTO batnav_projet.carte VALUES (197, 97, 103, 'C_OUPS');
INSERT INTO batnav_projet.carte VALUES (198, 98, 103, 'C_OUPS');
INSERT INTO batnav_projet.carte VALUES (199, 99, 103, 'C_OUPS');
INSERT INTO batnav_projet.carte VALUES (200, 100, 103, 'C_OUPS');


--
-- Data for Name: cellule_devoile; Type: TABLE DATA; Schema: batnav_projet; Owner: p2401496
--

INSERT INTO batnav_projet.cellule_devoile VALUES (52, 1, 1, 1, 1, 'A');
INSERT INTO batnav_projet.cellule_devoile VALUES (52, 1, 2, 1, 5, 'D');
INSERT INTO batnav_projet.cellule_devoile VALUES (52, 1, 3, 1, 6, 'D');
INSERT INTO batnav_projet.cellule_devoile VALUES (52, 2, 1, 1, 8, 'B');
INSERT INTO batnav_projet.cellule_devoile VALUES (52, 2, 2, 1, 8, 'B');
INSERT INTO batnav_projet.cellule_devoile VALUES (63, 1, 1, 2, 4, 'F');
INSERT INTO batnav_projet.cellule_devoile VALUES (63, 1, 2, 2, 6, 'C');
INSERT INTO batnav_projet.cellule_devoile VALUES (41, 1, 1, 3, 3, 'C');
INSERT INTO batnav_projet.cellule_devoile VALUES (41, 1, 2, 3, 6, 'A');
INSERT INTO batnav_projet.cellule_devoile VALUES (41, 2, 1, 3, 3, 'C');
INSERT INTO batnav_projet.cellule_devoile VALUES (41, 3, 1, 3, 3, 'D');
INSERT INTO batnav_projet.cellule_devoile VALUES (41, 3, 2, 3, 7, 'E');


--
-- Data for Name: cellule_grille; Type: TABLE DATA; Schema: batnav_projet; Owner: p2401496
--



--
-- Data for Name: classement; Type: TABLE DATA; Schema: batnav_projet; Owner: p2401496
--



--
-- Data for Name: classement_cpp; Type: TABLE DATA; Schema: batnav_projet; Owner: p2401496
--



--
-- Data for Name: classement_ijh; Type: TABLE DATA; Schema: batnav_projet; Owner: p2401496
--



--
-- Data for Name: dispose_grille; Type: TABLE DATA; Schema: batnav_projet; Owner: p2401496
--

INSERT INTO batnav_projet.dispose_grille VALUES (1, 6, 52);
INSERT INTO batnav_projet.dispose_grille VALUES (4, 5, 63);
INSERT INTO batnav_projet.dispose_grille VALUES (1, 1, 52);
INSERT INTO batnav_projet.dispose_grille VALUES (4, 2, 63);
INSERT INTO batnav_projet.dispose_grille VALUES (1, 3, 41);
INSERT INTO batnav_projet.dispose_grille VALUES (2, 4, 41);
INSERT INTO batnav_projet.dispose_grille VALUES (3, 2, 63);
INSERT INTO batnav_projet.dispose_grille VALUES (1, 4, 41);
INSERT INTO batnav_projet.dispose_grille VALUES (2, 3, 41);
INSERT INTO batnav_projet.dispose_grille VALUES (3, 1, 52);
INSERT INTO batnav_projet.dispose_grille VALUES (3, 5, 63);
INSERT INTO batnav_projet.dispose_grille VALUES (3, 6, 52);


--
-- Data for Name: distribution_carte; Type: TABLE DATA; Schema: batnav_projet; Owner: p2401496
--

INSERT INTO batnav_projet.distribution_carte VALUES (1, 'Distrib 1', 50, 10, 5, 5, 3, 3, 3, 1, 10, 5);
INSERT INTO batnav_projet.distribution_carte VALUES (2, 'Distrib 2', 20, 15, 13, 10, 5, 5, 5, 2, 15, 10);
INSERT INTO batnav_projet.distribution_carte VALUES (3, 'Distrib 3', 10, 5, 15, 10, 10, 10, 10, 5, 5, 20);


--
-- Data for Name: est_place; Type: TABLE DATA; Schema: batnav_projet; Owner: p2401496
--

INSERT INTO batnav_projet.est_place VALUES (63, 6303, 32, 8, 'F', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (63, 6304, 32, 9, 'F', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (52, 5203, 32, 8, 'F', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (52, 5201, 32, 9, 'F', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (41, 4102, 32, 8, 'F', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (41, 4101, 32, 9, 'F', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (63, 6303, 33, 7, 'E', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (63, 6304, 33, 8, 'D', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (52, 5203, 33, 7, 'E', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (52, 5201, 33, 8, 'D', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (41, 4102, 33, 7, 'E', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (41, 4101, 33, 8, 'D', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (63, 6303, 34, 5, 'D', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (63, 6304, 34, 4, 'C', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (52, 5203, 34, 5, 'D', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (52, 5201, 34, 4, 'C', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (41, 4102, 34, 5, 'D', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (41, 4101, 34, 4, 'C', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (63, 6303, 36, 5, 'D', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (63, 6304, 36, 4, 'C', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (52, 5203, 36, 5, 'D', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (52, 5201, 36, 4, 'C', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (41, 4102, 36, 5, 'D', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (41, 4101, 36, 4, 'C', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (63, 6303, 38, 7, 'E', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (63, 6304, 38, 8, 'D', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (52, 5203, 38, 7, 'E', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (52, 5201, 38, 8, 'D', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (41, 4102, 38, 7, 'E', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (41, 4101, 38, 8, 'D', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (63, 6303, 39, 5, 'D', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (63, 6304, 39, 4, 'C', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (52, 5203, 39, 5, 'D', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (52, 5201, 39, 4, 'C', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (41, 4102, 39, 5, 'D', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (41, 4101, 39, 4, 'C', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (63, 6303, 40, 2, 'I', 'vertical', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (63, 6304, 40, 1, 'G', 'vertical', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (52, 5203, 40, 2, 'I', 'vertical', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (52, 5201, 40, 1, 'G', 'vertical', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (41, 4102, 40, 2, 'I', 'vertical', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (41, 4101, 40, 1, 'G', 'vertical', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (63, 6303, 41, 5, 'D', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (63, 6304, 41, 4, 'C', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (52, 5203, 41, 5, 'D', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (52, 5201, 41, 4, 'C', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (41, 4102, 41, 5, 'D', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (41, 4101, 41, 4, 'C', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (63, 6303, 42, 2, 'I', 'vertical', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (63, 6304, 42, 1, 'G', 'vertical', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (52, 5203, 42, 2, 'I', 'vertical', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (52, 5201, 42, 1, 'G', 'vertical', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (41, 4102, 42, 2, 'I', 'vertical', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (41, 4101, 42, 1, 'G', 'vertical', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (63, 6303, 43, 7, 'E', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (63, 6304, 43, 8, 'D', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (52, 5203, 43, 7, 'E', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (52, 5201, 43, 8, 'D', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (41, 4102, 43, 7, 'E', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (41, 4101, 43, 8, 'D', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (63, 6303, 44, 8, 'F', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (63, 6304, 44, 9, 'F', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (52, 5203, 44, 8, 'F', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (52, 5201, 44, 9, 'F', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (41, 4102, 44, 8, 'F', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (41, 4101, 44, 9, 'F', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (63, 6303, 45, 2, 'I', 'vertical', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (63, 6304, 45, 1, 'G', 'vertical', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (52, 5203, 45, 2, 'I', 'vertical', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (52, 5201, 45, 1, 'G', 'vertical', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (41, 4102, 45, 2, 'I', 'vertical', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (41, 4101, 45, 1, 'G', 'vertical', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (63, 6303, 47, 8, 'F', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (63, 6304, 47, 9, 'F', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (52, 5203, 47, 8, 'F', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (52, 5201, 47, 9, 'F', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (41, 4102, 47, 8, 'F', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (41, 4101, 47, 9, 'F', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (63, 6303, 48, 8, 'F', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (63, 6304, 48, 9, 'F', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (52, 5203, 48, 8, 'F', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (52, 5201, 48, 9, 'F', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (41, 4102, 48, 8, 'F', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (41, 4101, 48, 9, 'F', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (63, 6303, 50, 2, 'I', 'vertical', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (63, 6304, 50, 1, 'G', 'vertical', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (52, 5203, 50, 2, 'I', 'vertical', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (52, 5201, 50, 1, 'G', 'vertical', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (41, 4102, 50, 2, 'I', 'vertical', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (41, 4101, 50, 1, 'G', 'vertical', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (63, 6303, 51, 7, 'E', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (63, 6304, 51, 8, 'D', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (52, 5203, 51, 7, 'E', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (52, 5201, 51, 8, 'D', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (41, 4102, 51, 7, 'E', 'horizontal', 'opérationnel');
INSERT INTO batnav_projet.est_place VALUES (41, 4101, 51, 8, 'D', 'horizontal', 'opérationnel');


--
-- Data for Name: flottille; Type: TABLE DATA; Schema: batnav_projet; Owner: p2401496
--

INSERT INTO batnav_projet.flottille VALUES (1, 'lottille de coalition', 5);
INSERT INTO batnav_projet.flottille VALUES (2, 'lottille de coalition', 5);
INSERT INTO batnav_projet.flottille VALUES (3, 'lottille nationale', 5);
INSERT INTO batnav_projet.flottille VALUES (4, 'lottille nationale', 5);


--
-- Data for Name: grille; Type: TABLE DATA; Schema: batnav_projet; Owner: p2401496
--

INSERT INTO batnav_projet.grille VALUES (1, 52, 1, 10, 10, 'img/cellule_vide.png', 'img/cellule_eau.png', 'img/cellule_touche.png');
INSERT INTO batnav_projet.grille VALUES (2, 63, 1, 10, 10, 'img/cellule_vide.png', 'img/cellule_eau.png', 'img/cellule_touche.png');
INSERT INTO batnav_projet.grille VALUES (3, 41, 1, 10, 10, 'img/cellule_vide.png', 'img/cellule_eau.png', 'img/cellule_touche.png');
INSERT INTO batnav_projet.grille VALUES (4, 41, 2, 10, 10, 'img/cellule_vide.png', 'img/cellule_eau.png', 'img/cellule_touche.png');
INSERT INTO batnav_projet.grille VALUES (5, 63, 2, 10, 10, 'img/cellule_vide.png', 'img/cellule_eau.png', 'img/cellule_touche.png');
INSERT INTO batnav_projet.grille VALUES (6, 52, 2, 10, 10, 'img/cellule_vide.png', 'img/cellule_eau.png', 'img/cellule_touche.png');


--
-- Data for Name: joueur; Type: TABLE DATA; Schema: batnav_projet; Owner: p2401496
--

INSERT INTO batnav_projet.joueur VALUES (1, 'Yacou');
INSERT INTO batnav_projet.joueur VALUES (2, 'Zelda');
INSERT INTO batnav_projet.joueur VALUES (3, 'Mario');
INSERT INTO batnav_projet.joueur VALUES (4, 'You');
INSERT INTO batnav_projet.joueur VALUES (5, 'Kirby');
INSERT INTO batnav_projet.joueur VALUES (6, 'Pikachu');
INSERT INTO batnav_projet.joueur VALUES (7, 'JD');
INSERT INTO batnav_projet.joueur VALUES (8, 'evaB');
INSERT INTO batnav_projet.joueur VALUES (9, 'LaRo');
INSERT INTO batnav_projet.joueur VALUES (10, 'Thanos');
INSERT INTO batnav_projet.joueur VALUES (11, 'ET');


--
-- Data for Name: joueur_humain; Type: TABLE DATA; Schema: batnav_projet; Owner: p2401496
--

INSERT INTO batnav_projet.joueur_humain VALUES (1, 'ACOULAY', 'Yves', '2001-02-04', '2001-02-04');
INSERT INTO batnav_projet.joueur_humain VALUES (4, 'AITORPILLET', 'Youssef', '2002-12-23', '2002-12-23');
INSERT INTO batnav_projet.joueur_humain VALUES (7, 'DEAU', 'John', '1999-11-02', '1999-11-02');
INSERT INTO batnav_projet.joueur_humain VALUES (8, 'BOMBARDER', 'Eva', '1999-11-02', '1999-11-02');
INSERT INTO batnav_projet.joueur_humain VALUES (9, 'ROQUETTE', 'Lance', '1998-10-07', '1998-10-07');
INSERT INTO batnav_projet.joueur_humain VALUES (11, 'TOUCOULET', 'Ella', '2005-08-14', '2005-08-14');


--
-- Data for Name: joueur_virtuelle; Type: TABLE DATA; Schema: batnav_projet; Owner: p2401496
--

INSERT INTO batnav_projet.joueur_virtuelle VALUES (2, 'Expert', 1, '2026-03-03');
INSERT INTO batnav_projet.joueur_virtuelle VALUES (3, 'Faible', 4, '2026-03-04');
INSERT INTO batnav_projet.joueur_virtuelle VALUES (5, 'Faible', 4, '2026-03-06');
INSERT INTO batnav_projet.joueur_virtuelle VALUES (6, 'Intermédiaire', 7, '2026-03-07');
INSERT INTO batnav_projet.joueur_virtuelle VALUES (10, 'Expert', 11, '2026-03-11');


--
-- Data for Name: leurre; Type: TABLE DATA; Schema: batnav_projet; Owner: p2401496
--



--
-- Data for Name: navire; Type: TABLE DATA; Schema: batnav_projet; Owner: p2401496
--

INSERT INTO batnav_projet.navire VALUES (31, NULL, 'torpilleur', 2, 'FR');
INSERT INTO batnav_projet.navire VALUES (32, 'Le Terrible', 'contre-torpilleur', 3, 'EG');
INSERT INTO batnav_projet.navire VALUES (33, 'Le Triomphant', 'contre-torpilleur', 3, 'CH');
INSERT INTO batnav_projet.navire VALUES (34, 'Le Charles de Gaulle', 'porte-avion', 5, 'US');
INSERT INTO batnav_projet.navire VALUES (35, NULL, 'torpilleur', 2, 'MAR');
INSERT INTO batnav_projet.navire VALUES (36, 'Le Charles de Gaulle', 'porte-avion', 5, 'CH');
INSERT INTO batnav_projet.navire VALUES (37, NULL, 'torpilleur', 2, 'EG');
INSERT INTO batnav_projet.navire VALUES (38, 'Le Triomphant', 'contre-torpilleur', 3, 'MAR');
INSERT INTO batnav_projet.navire VALUES (39, 'Le Charles de Gaulle', 'porte-avion', 5, 'FR');
INSERT INTO batnav_projet.navire VALUES (40, 'Colbert', 'croiseur', 4, 'CH');
INSERT INTO batnav_projet.navire VALUES (41, 'Le Charles de Gaulle', 'porte-avion', 5, 'MAR');
INSERT INTO batnav_projet.navire VALUES (42, 'Colbert', 'croiseur', 4, 'UK');
INSERT INTO batnav_projet.navire VALUES (43, 'Le Triomphant', 'contre-torpilleur', 3, 'US');
INSERT INTO batnav_projet.navire VALUES (44, 'Le Terrible', 'contre-torpilleur', 3, 'UK');
INSERT INTO batnav_projet.navire VALUES (45, 'Colbert', 'croiseur', 4, 'US');
INSERT INTO batnav_projet.navire VALUES (46, NULL, 'torpilleur', 2, 'UK');
INSERT INTO batnav_projet.navire VALUES (47, 'Le Terrible', 'contre-torpilleur', 3, 'CH');
INSERT INTO batnav_projet.navire VALUES (48, 'Le Terrible', 'contre-torpilleur', 3, 'MAR');
INSERT INTO batnav_projet.navire VALUES (49, NULL, 'torpilleur', 2, 'US');
INSERT INTO batnav_projet.navire VALUES (50, 'Colbert', 'croiseur', 4, 'FR');
INSERT INTO batnav_projet.navire VALUES (51, 'Le Triomphant', 'contre-torpilleur', 3, 'UK');


--
-- Data for Name: orque; Type: TABLE DATA; Schema: batnav_projet; Owner: p2401496
--

INSERT INTO batnav_projet.orque VALUES (1, 'Willy', 'img/orque_willy.png');
INSERT INTO batnav_projet.orque VALUES (2, 'Nemo', 'img/orque_nemo.png');
INSERT INTO batnav_projet.orque VALUES (3, 'Flipper', 'img/orque_flipper.png');


--
-- Data for Name: partie; Type: TABLE DATA; Schema: batnav_projet; Owner: p2401496
--

INSERT INTO batnav_projet.partie VALUES (41, '2026-03-04', '10:05:00', 'Terminé', 140, 1, 2, 103, 1);
INSERT INTO batnav_projet.partie VALUES (52, '2026-03-13', '09:15:00', 'En cours', NULL, 1, 3, 105, NULL);
INSERT INTO batnav_projet.partie VALUES (63, '2026-03-10', '18:05:00', 'Suspendue', NULL, 4, 3, 103, NULL);


--
-- Data for Name: pavillion; Type: TABLE DATA; Schema: batnav_projet; Owner: p2401496
--

INSERT INTO batnav_projet.pavillion VALUES ('UK', 'Royaume-Uni');
INSERT INTO batnav_projet.pavillion VALUES ('FR', 'France');
INSERT INTO batnav_projet.pavillion VALUES ('US', 'Etats-Unis');
INSERT INTO batnav_projet.pavillion VALUES ('CH', 'Chine');
INSERT INTO batnav_projet.pavillion VALUES ('EG', 'Egypte');
INSERT INTO batnav_projet.pavillion VALUES ('MAR', 'Maroc');


--
-- Data for Name: peut_contenir; Type: TABLE DATA; Schema: batnav_projet; Owner: p2401496
--



--
-- Data for Name: pioche; Type: TABLE DATA; Schema: batnav_projet; Owner: p2401496
--

INSERT INTO batnav_projet.pioche VALUES (103, 1);
INSERT INTO batnav_projet.pioche VALUES (105, 3);


--
-- Data for Name: sequence_temporelle; Type: TABLE DATA; Schema: batnav_projet; Owner: p2401496
--

INSERT INTO batnav_projet.sequence_temporelle VALUES (41, 1, '2026-03-04', '10:05:00', '2026-03-04', '10:35:00');
INSERT INTO batnav_projet.sequence_temporelle VALUES (52, 1, '2026-03-13', '09:15:00', NULL, NULL);
INSERT INTO batnav_projet.sequence_temporelle VALUES (63, 1, '2026-03-10', '18:05:00', '2026-03-10', '19:05:00');
INSERT INTO batnav_projet.sequence_temporelle VALUES (41, 2, '2026-03-04', '14:00:00', '2026-03-04', '15:32:00');


--
-- Data for Name: tir; Type: TABLE DATA; Schema: batnav_projet; Owner: p2401496
--

INSERT INTO batnav_projet.tir VALUES (41, 1, 1, 3, 'C', 1, 12);
INSERT INTO batnav_projet.tir VALUES (41, 1, 2, 6, 'A', 2, 24);
INSERT INTO batnav_projet.tir VALUES (41, 2, 1, 3, 'C', 1, 10);
INSERT INTO batnav_projet.tir VALUES (41, 2, 2, NULL, NULL, 2, 66);
INSERT INTO batnav_projet.tir VALUES (41, 3, 1, 3, 'D', 1, 6);
INSERT INTO batnav_projet.tir VALUES (41, 3, 2, 7, 'E', 2, 16);
INSERT INTO batnav_projet.tir VALUES (52, 1, 1, 1, 'A', 1, 6);
INSERT INTO batnav_projet.tir VALUES (52, 1, 2, 5, 'D', 3, 14);
INSERT INTO batnav_projet.tir VALUES (52, 1, 3, 6, 'D', 3, 25);
INSERT INTO batnav_projet.tir VALUES (52, 2, 1, 8, 'B', 1, 11);
INSERT INTO batnav_projet.tir VALUES (52, 2, 2, 8, 'B', 3, 60);
INSERT INTO batnav_projet.tir VALUES (63, 1, 1, 4, 'F', 4, 18);
INSERT INTO batnav_projet.tir VALUES (63, 1, 2, 6, 'C', 3, 50);


--
-- Data for Name: tour; Type: TABLE DATA; Schema: batnav_projet; Owner: p2401496
--

INSERT INTO batnav_projet.tour VALUES (41, 1, NULL, NULL, NULL, NULL, NULL, NULL);
INSERT INTO batnav_projet.tour VALUES (41, 2, NULL, NULL, NULL, NULL, NULL, NULL);
INSERT INTO batnav_projet.tour VALUES (41, 3, NULL, NULL, NULL, NULL, NULL, NULL);
INSERT INTO batnav_projet.tour VALUES (52, 1, NULL, NULL, NULL, NULL, NULL, NULL);
INSERT INTO batnav_projet.tour VALUES (52, 2, NULL, NULL, NULL, NULL, NULL, NULL);
INSERT INTO batnav_projet.tour VALUES (63, 1, NULL, NULL, NULL, NULL, NULL, NULL);


--
-- Data for Name: type_carte; Type: TABLE DATA; Schema: batnav_projet; Owner: p2401496
--

INSERT INTO batnav_projet.type_carte VALUES ('C_MISSILE', false, 'Missile', 'Cette carte permet de tirer un missile "classique" qui touchera la case indiquée', 'carte1.png');
INSERT INTO batnav_projet.type_carte VALUES ('C_REJOUE', true, 'Rejoue une fois', 'Cette carte  permet à celui qui la tire de tirer dans le même tour une seconde fois', 'carte2.png');
INSERT INTO batnav_projet.type_carte VALUES ('C_VIDE', true, 'Vide ou pas vide?', 'Cette carte permet à celui qui la tire de savoir avant de lancer son missile si une case est vide ou pas par une notification. Cela lui permet de confirmer le tir où de changer de coordonnées.', 'carte3.png');
INSERT INTO batnav_projet.type_carte VALUES ('C_MPM', true, 'Même pas mal', 'Cette carte permet à celui qui la tire d''annuler un dégât subi sur un de ses navires et de le changer de place', 'carte4.png');
INSERT INTO batnav_projet.type_carte VALUES ('C_LEURRE', true, 'Bâteau leurre', 'Cette carte permet à celui qui la tire de placer un bateau leurre dans la grille. Si durant la partie le leurre est touché, il disparaît en laissant croire à l''adversaire qu''il a touché un navire ', 'carte5.png');
INSERT INTO batnav_projet.type_carte VALUES ('C_WILLY', true, 'Sauvez Willy', 'Cette carte permet à celui qui la tire de placer une orque dans la grille. Si durant la partie l''orque est touchée, les trois plus petits navires non encore coulés sont alors coulés en représailles des activistes écologistes
', 'carte6.png');
INSERT INTO batnav_projet.type_carte VALUES ('C_MEGA', true, 'Méga-bombe', 'Cette carte ermet à celui qui la tire de remplacer son prochain missile par une méga-bombe qui fera des dégâts également sur les 8 cases adjacentes à la cible, soit 9 cases d''un coup', 'carte7.png');
INSERT INTO batnav_projet.type_carte VALUES ('C_ETOILE', true, 'Étoile de la mort', 'Cette carte permet à celui qui la tire de remplacer son prochain missile par un tir depuis l''étoile de la mort qui fera des dégâts également sur les 24 cases les plus proches de la cible, soit 25 cases d''un coup', 'carte8.png');
INSERT INTO batnav_projet.type_carte VALUES ('C_PASSE', false, 'Passe ton tour', 'Le joueur qui pioche cette carte perd son tour. Son adversaire joue donc deux fois de suite.', 'carte9.png');
INSERT INTO batnav_projet.type_carte VALUES ('C_OUPS', false, 'Mauvaise manip', 'Cette carte fait que le missile que vous vous apprêtiez à tirer a été échappé par le matelot qui devait le charger dans le silo. C''est donc un de vos navires, choisi aléatoirement qui est touché et vous perdez votre tour', 'carte10.png');


--
-- Name: appartient_flottille_id_f_seq; Type: SEQUENCE SET; Schema: batnav_projet; Owner: p2401496
--

SELECT pg_catalog.setval('batnav_projet.appartient_flottille_id_f_seq', 1, false);


--
-- Name: appartient_flottille_id_n_seq; Type: SEQUENCE SET; Schema: batnav_projet; Owner: p2401496
--

SELECT pg_catalog.setval('batnav_projet.appartient_flottille_id_n_seq', 1, false);


--
-- Name: carte_id_carte_seq; Type: SEQUENCE SET; Schema: batnav_projet; Owner: p2401496
--

SELECT pg_catalog.setval('batnav_projet.carte_id_carte_seq', 1, false);


--
-- Name: cellule_grille_id_n_seq; Type: SEQUENCE SET; Schema: batnav_projet; Owner: p2401496
--

SELECT pg_catalog.setval('batnav_projet.cellule_grille_id_n_seq', 1, false);


--
-- Name: classement_ijh_id_j_seq; Type: SEQUENCE SET; Schema: batnav_projet; Owner: p2401496
--

SELECT pg_catalog.setval('batnav_projet.classement_ijh_id_j_seq', 1, false);


--
-- Name: dispose_grille_id_g_seq; Type: SEQUENCE SET; Schema: batnav_projet; Owner: p2401496
--

SELECT pg_catalog.setval('batnav_projet.dispose_grille_id_g_seq', 1, false);


--
-- Name: dispose_grille_id_j_seq; Type: SEQUENCE SET; Schema: batnav_projet; Owner: p2401496
--

SELECT pg_catalog.setval('batnav_projet.dispose_grille_id_j_seq', 1, false);


--
-- Name: distribution_carte_id_d_seq; Type: SEQUENCE SET; Schema: batnav_projet; Owner: p2401496
--

SELECT pg_catalog.setval('batnav_projet.distribution_carte_id_d_seq', 1, false);


--
-- Name: est_place_id_n_seq; Type: SEQUENCE SET; Schema: batnav_projet; Owner: p2401496
--

SELECT pg_catalog.setval('batnav_projet.est_place_id_n_seq', 1, false);


--
-- Name: flottille_id_f_seq; Type: SEQUENCE SET; Schema: batnav_projet; Owner: p2401496
--

SELECT pg_catalog.setval('batnav_projet.flottille_id_f_seq', 4, true);


--
-- Name: grille_id_g_seq; Type: SEQUENCE SET; Schema: batnav_projet; Owner: p2401496
--

SELECT pg_catalog.setval('batnav_projet.grille_id_g_seq', 6, true);


--
-- Name: joueur_humain_id_j_seq; Type: SEQUENCE SET; Schema: batnav_projet; Owner: p2401496
--

SELECT pg_catalog.setval('batnav_projet.joueur_humain_id_j_seq', 1, false);


--
-- Name: joueur_id_j_seq; Type: SEQUENCE SET; Schema: batnav_projet; Owner: p2401496
--

SELECT pg_catalog.setval('batnav_projet.joueur_id_j_seq', 1, false);


--
-- Name: joueur_virtuelle_id_j_createur_seq; Type: SEQUENCE SET; Schema: batnav_projet; Owner: p2401496
--

SELECT pg_catalog.setval('batnav_projet.joueur_virtuelle_id_j_createur_seq', 1, false);


--
-- Name: joueur_virtuelle_id_j_seq; Type: SEQUENCE SET; Schema: batnav_projet; Owner: p2401496
--

SELECT pg_catalog.setval('batnav_projet.joueur_virtuelle_id_j_seq', 1, false);


--
-- Name: navire_id_n_seq; Type: SEQUENCE SET; Schema: batnav_projet; Owner: p2401496
--

SELECT pg_catalog.setval('batnav_projet.navire_id_n_seq', 51, true);


--
-- Name: orque_id_q_seq; Type: SEQUENCE SET; Schema: batnav_projet; Owner: p2401496
--

SELECT pg_catalog.setval('batnav_projet.orque_id_q_seq', 3, true);


--
-- Name: partie_id_j_humain_seq; Type: SEQUENCE SET; Schema: batnav_projet; Owner: p2401496
--

SELECT pg_catalog.setval('batnav_projet.partie_id_j_humain_seq', 1, false);


--
-- Name: partie_id_j_virtuelle_seq; Type: SEQUENCE SET; Schema: batnav_projet; Owner: p2401496
--

SELECT pg_catalog.setval('batnav_projet.partie_id_j_virtuelle_seq', 1, false);


--
-- Name: peut_contenir_id_q_seq; Type: SEQUENCE SET; Schema: batnav_projet; Owner: p2401496
--

SELECT pg_catalog.setval('batnav_projet.peut_contenir_id_q_seq', 1, false);


--
-- Name: pioche_id_d_seq; Type: SEQUENCE SET; Schema: batnav_projet; Owner: p2401496
--

SELECT pg_catalog.setval('batnav_projet.pioche_id_d_seq', 1, false);


--
-- Name: tir_id_carte_seq; Type: SEQUENCE SET; Schema: batnav_projet; Owner: p2401496
--

SELECT pg_catalog.setval('batnav_projet.tir_id_carte_seq', 1, false);


--
-- Name: tir_id_j_seq; Type: SEQUENCE SET; Schema: batnav_projet; Owner: p2401496
--

SELECT pg_catalog.setval('batnav_projet.tir_id_j_seq', 1, false);


--
-- Name: appartient_flottille appartient_flottille_pkey; Type: CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.appartient_flottille
    ADD CONSTRAINT appartient_flottille_pkey PRIMARY KEY (id_f, id_n);


--
-- Name: carte carte_pkey; Type: CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.carte
    ADD CONSTRAINT carte_pkey PRIMARY KEY (id_carte);


--
-- Name: cellule_devoile cellule_devoile_pkey; Type: CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.cellule_devoile
    ADD CONSTRAINT cellule_devoile_pkey PRIMARY KEY (id_p, num_t, num_ti, id_g, coord_x, coord_y);


--
-- Name: cellule_grille cellule_grille_pkey; Type: CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.cellule_grille
    ADD CONSTRAINT cellule_grille_pkey PRIMARY KEY (id_g, coord_x, coord_y);


--
-- Name: classement_cpp classement_cpp_pkey; Type: CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.classement_cpp
    ADD CONSTRAINT classement_cpp_pkey PRIMARY KEY (id_c, code_pays);


--
-- Name: classement_ijh classement_ijh_pkey; Type: CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.classement_ijh
    ADD CONSTRAINT classement_ijh_pkey PRIMARY KEY (id_j, rang);


--
-- Name: classement classement_pkey; Type: CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.classement
    ADD CONSTRAINT classement_pkey PRIMARY KEY (id_c);


--
-- Name: dispose_grille dispose_grille_pkey; Type: CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.dispose_grille
    ADD CONSTRAINT dispose_grille_pkey PRIMARY KEY (id_j, id_g, id_p);


--
-- Name: distribution_carte distribution_carte_pkey; Type: CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.distribution_carte
    ADD CONSTRAINT distribution_carte_pkey PRIMARY KEY (id_d);


--
-- Name: est_place est_place_pkey; Type: CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.est_place
    ADD CONSTRAINT est_place_pkey PRIMARY KEY (id_p, id_g, id_n);


--
-- Name: flottille flottille_pkey; Type: CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.flottille
    ADD CONSTRAINT flottille_pkey PRIMARY KEY (id_f);


--
-- Name: grille grille_pkey; Type: CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.grille
    ADD CONSTRAINT grille_pkey PRIMARY KEY (id_g, id_p);


--
-- Name: joueur_humain joueur_humain_pkey; Type: CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.joueur_humain
    ADD CONSTRAINT joueur_humain_pkey PRIMARY KEY (id_j);


--
-- Name: joueur joueur_pkey; Type: CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.joueur
    ADD CONSTRAINT joueur_pkey PRIMARY KEY (id_j);


--
-- Name: joueur_virtuelle joueur_virtuelle_pkey; Type: CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.joueur_virtuelle
    ADD CONSTRAINT joueur_virtuelle_pkey PRIMARY KEY (id_j);


--
-- Name: leurre leurre_pkey; Type: CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.leurre
    ADD CONSTRAINT leurre_pkey PRIMARY KEY (id_g, num_l);


--
-- Name: navire navire_pkey; Type: CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.navire
    ADD CONSTRAINT navire_pkey PRIMARY KEY (id_n);


--
-- Name: orque orque_pkey; Type: CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.orque
    ADD CONSTRAINT orque_pkey PRIMARY KEY (id_q);


--
-- Name: partie partie_pkey; Type: CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.partie
    ADD CONSTRAINT partie_pkey PRIMARY KEY (id_p);


--
-- Name: pavillion pavillion_pkey; Type: CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.pavillion
    ADD CONSTRAINT pavillion_pkey PRIMARY KEY (code_pays);


--
-- Name: peut_contenir peut_contenir_pkey; Type: CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.peut_contenir
    ADD CONSTRAINT peut_contenir_pkey PRIMARY KEY (id_g, id_q);


--
-- Name: pioche pioche_pkey; Type: CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.pioche
    ADD CONSTRAINT pioche_pkey PRIMARY KEY (id_pi);


--
-- Name: sequence_temporelle sequence_temporelle_pkey; Type: CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.sequence_temporelle
    ADD CONSTRAINT sequence_temporelle_pkey PRIMARY KEY (id_p, num_seq);


--
-- Name: tir tir_pkey; Type: CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.tir
    ADD CONSTRAINT tir_pkey PRIMARY KEY (id_p, num_t, num_ti);


--
-- Name: tour tour_pkey; Type: CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.tour
    ADD CONSTRAINT tour_pkey PRIMARY KEY (id_p, num_t);


--
-- Name: type_carte type_carte_pkey; Type: CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.type_carte
    ADD CONSTRAINT type_carte_pkey PRIMARY KEY (code_carte);


--
-- Name: appartient_flottille appartient_flottille_id_f_fkey; Type: FK CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.appartient_flottille
    ADD CONSTRAINT appartient_flottille_id_f_fkey FOREIGN KEY (id_f) REFERENCES batnav_projet.flottille(id_f);


--
-- Name: appartient_flottille appartient_flottille_id_n_fkey; Type: FK CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.appartient_flottille
    ADD CONSTRAINT appartient_flottille_id_n_fkey FOREIGN KEY (id_n) REFERENCES batnav_projet.navire(id_n);


--
-- Name: carte carte_code_carte_fkey; Type: FK CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.carte
    ADD CONSTRAINT carte_code_carte_fkey FOREIGN KEY (code_carte) REFERENCES batnav_projet.type_carte(code_carte);


--
-- Name: carte carte_id_pi_fkey; Type: FK CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.carte
    ADD CONSTRAINT carte_id_pi_fkey FOREIGN KEY (id_pi) REFERENCES batnav_projet.pioche(id_pi);


--
-- Name: cellule_devoile cellule_devoile_id_p_num_t_num_ti_fkey; Type: FK CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.cellule_devoile
    ADD CONSTRAINT cellule_devoile_id_p_num_t_num_ti_fkey FOREIGN KEY (id_p, num_t, num_ti) REFERENCES batnav_projet.tir(id_p, num_t, num_ti);


--
-- Name: cellule_grille cellule_grille_id_n_fkey; Type: FK CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.cellule_grille
    ADD CONSTRAINT cellule_grille_id_n_fkey FOREIGN KEY (id_n) REFERENCES batnav_projet.navire(id_n);


--
-- Name: classement_cpp classement_cpp_code_pays_fkey; Type: FK CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.classement_cpp
    ADD CONSTRAINT classement_cpp_code_pays_fkey FOREIGN KEY (code_pays) REFERENCES batnav_projet.pavillion(code_pays);


--
-- Name: classement_cpp classement_cpp_id_c_fkey; Type: FK CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.classement_cpp
    ADD CONSTRAINT classement_cpp_id_c_fkey FOREIGN KEY (id_c) REFERENCES batnav_projet.classement(id_c);


--
-- Name: classement_ijh classement_ijh_id_c_fkey; Type: FK CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.classement_ijh
    ADD CONSTRAINT classement_ijh_id_c_fkey FOREIGN KEY (id_c) REFERENCES batnav_projet.classement(id_c);


--
-- Name: classement_ijh classement_ijh_id_j_fkey; Type: FK CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.classement_ijh
    ADD CONSTRAINT classement_ijh_id_j_fkey FOREIGN KEY (id_j) REFERENCES batnav_projet.joueur(id_j);


--
-- Name: dispose_grille dispose_grille_id_j_fkey; Type: FK CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.dispose_grille
    ADD CONSTRAINT dispose_grille_id_j_fkey FOREIGN KEY (id_j) REFERENCES batnav_projet.joueur(id_j);


--
-- Name: dispose_grille dispose_grille_id_p_fkey; Type: FK CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.dispose_grille
    ADD CONSTRAINT dispose_grille_id_p_fkey FOREIGN KEY (id_p) REFERENCES batnav_projet.partie(id_p);


--
-- Name: est_place est_place_id_n_fkey; Type: FK CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.est_place
    ADD CONSTRAINT est_place_id_n_fkey FOREIGN KEY (id_n) REFERENCES batnav_projet.navire(id_n);


--
-- Name: joueur_humain joueur_humain_id_j_fkey; Type: FK CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.joueur_humain
    ADD CONSTRAINT joueur_humain_id_j_fkey FOREIGN KEY (id_j) REFERENCES batnav_projet.joueur(id_j);


--
-- Name: joueur_virtuelle joueur_virtuelle_id_j_createur_fkey; Type: FK CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.joueur_virtuelle
    ADD CONSTRAINT joueur_virtuelle_id_j_createur_fkey FOREIGN KEY (id_j_createur) REFERENCES batnav_projet.joueur_humain(id_j);


--
-- Name: joueur_virtuelle joueur_virtuelle_id_j_fkey; Type: FK CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.joueur_virtuelle
    ADD CONSTRAINT joueur_virtuelle_id_j_fkey FOREIGN KEY (id_j) REFERENCES batnav_projet.joueur(id_j);


--
-- Name: navire navire_code_pays_fkey; Type: FK CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.navire
    ADD CONSTRAINT navire_code_pays_fkey FOREIGN KEY (code_pays) REFERENCES batnav_projet.pavillion(code_pays);


--
-- Name: peut_contenir peut_contenir_id_q_fkey; Type: FK CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.peut_contenir
    ADD CONSTRAINT peut_contenir_id_q_fkey FOREIGN KEY (id_q) REFERENCES batnav_projet.orque(id_q);


--
-- Name: pioche pioche_id_d_fkey; Type: FK CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.pioche
    ADD CONSTRAINT pioche_id_d_fkey FOREIGN KEY (id_d) REFERENCES batnav_projet.distribution_carte(id_d);


--
-- Name: tir tir_id_carte_fkey; Type: FK CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.tir
    ADD CONSTRAINT tir_id_carte_fkey FOREIGN KEY (id_carte) REFERENCES batnav_projet.carte(id_carte);


--
-- Name: tir tir_id_j_fkey; Type: FK CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.tir
    ADD CONSTRAINT tir_id_j_fkey FOREIGN KEY (id_j) REFERENCES batnav_projet.joueur(id_j);


--
-- Name: tir tir_id_p_num_t_fkey; Type: FK CONSTRAINT; Schema: batnav_projet; Owner: p2401496
--

ALTER TABLE ONLY batnav_projet.tir
    ADD CONSTRAINT tir_id_p_num_t_fkey FOREIGN KEY (id_p, num_t) REFERENCES batnav_projet.tour(id_p, num_t);


--
-- PostgreSQL database dump complete
--

