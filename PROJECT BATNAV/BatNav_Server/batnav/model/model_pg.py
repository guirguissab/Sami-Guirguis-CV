import math
import random

import psycopg
from psycopg.rows import dict_row
from logzero import logger


LETTERS = list("ABCDEFGHIJ")
STATIC_IMG_PREFIX = "/batnav/static/img/"
GRID_IMAGE_MAP_SHIP = {
    "?": f"{STATIC_IMG_PREFIX}cellule_non_exploree_1.png",
    "N": f"{STATIC_IMG_PREFIX}cellule_navire.png",
    "T": f"{STATIC_IMG_PREFIX}cellule_touche.png",
    "P": f"{STATIC_IMG_PREFIX}cellule_eau.png",
}
GRID_IMAGE_MAP_SHOT = {
    "?": f"{STATIC_IMG_PREFIX}cellule_non_exploree_2.png",
    "N": f"{STATIC_IMG_PREFIX}cellule_navire.png",
    "T": f"{STATIC_IMG_PREFIX}cellule_touche.png",
    "P": f"{STATIC_IMG_PREFIX}cellule_eau.png",
}
SIMPLE_FLEET_LAYOUT = [
    {"nom": "Le Foch", "type_navire": "porte-avion", "taille": 5, "coord_x": 1, "coord_y": "A", "sens": "horizontal"},
    {"nom": "Colbert", "type_navire": "croiseur", "taille": 4, "coord_x": 3, "coord_y": "C", "sens": "vertical"},
    {"nom": "Le Terrible", "type_navire": "contre-torpilleur", "taille": 3, "coord_x": 6, "coord_y": "F", "sens": "horizontal"},
    {"nom": "Le Triomphant", "type_navire": "contre-torpilleur", "taille": 3, "coord_x": 8, "coord_y": "B", "sens": "horizontal"},
    {"nom": "Torpilleur", "type_navire": "torpilleur", "taille": 2, "coord_x": 10, "coord_y": "I", "sens": "horizontal"},
]
SIMPLE_FLEET_LAYOUT_AI = [
    {"nom": "Porte-avion IA", "type_navire": "porte-avion", "taille": 5, "coord_x": 2, "coord_y": "B", "sens": "horizontal"},
    {"nom": "Croiseur IA", "type_navire": "croiseur", "taille": 4, "coord_x": 5, "coord_y": "E", "sens": "vertical"},
    {"nom": "CT IA 1", "type_navire": "contre-torpilleur", "taille": 3, "coord_x": 7, "coord_y": "H", "sens": "horizontal"},
    {"nom": "CT IA 2", "type_navire": "contre-torpilleur", "taille": 3, "coord_x": 9, "coord_y": "C", "sens": "horizontal"},
    {"nom": "Torpilleur IA", "type_navire": "torpilleur", "taille": 2, "coord_x": 1, "coord_y": "J", "sens": "vertical"},
]


def execute_select_query(connexion, query, params=None):
    """
    Exécute une requête SELECT et retourne une liste de dictionnaires.
    """
    params = params or []
    try:
        with connexion.cursor(row_factory=dict_row) as cursor:
            cursor.execute(query, params)
            return cursor.fetchall()
    except psycopg.Error as e:
        logger.error(e)
        return None


def execute_one_query(connexion, query, params=None):
    """
    Exécute une requête SELECT et retourne la première ligne.
    """
    result = execute_select_query(connexion, query, params)
    if result:
        return result[0]
    return None


def search_human_players_by_pseudo(connexion, pseudo_pattern):
    """
    Recherche les joueurs humains dont le pseudo contient pseudo_pattern.
    """
    motif = f"%{pseudo_pattern}%"
    query = """
        SELECT
            jh.id_j,
            j.pseudo,
            jh.nom,
            jh.prenom,
            TO_CHAR(jh.date_naiss, 'DD/MM/YYYY') AS date_naiss
        FROM batnav_projet.joueur_humain AS jh
        JOIN batnav_projet.joueur AS j ON j.id_j = jh.id_j
        WHERE j.pseudo ILIKE %s
        ORDER BY j.pseudo
    """
    return execute_select_query(connexion, query, [motif])


def get_human_player_by_id(connexion, id_joueur):
    """
    Retourne les informations d'un joueur humain à partir de son identifiant.
    """
    query = """
        SELECT
            jh.id_j,
            j.pseudo,
            jh.nom,
            jh.prenom,
            TO_CHAR(jh.date_naiss, 'DD/MM/YYYY') AS date_naiss
        FROM batnav_projet.joueur_humain AS jh
        JOIN batnav_projet.joueur AS j ON j.id_j = jh.id_j
        WHERE jh.id_j = %s
    """
    return execute_one_query(connexion, query, [id_joueur])


def insert_human_player(connexion, pseudo, nom, prenom, date_naissance):
    """
    Crée un joueur humain.
    """
    with connexion.cursor() as cursor:
        try:
            cursor.execute("SELECT COALESCE(MAX(id_j), 0) + 1 AS next_id FROM batnav_projet.joueur")
            next_id = cursor.fetchone()[0]
            cursor.execute(
                "INSERT INTO batnav_projet.joueur(id_j, pseudo) VALUES (%s, %s)",
                [next_id, pseudo],
            )
            cursor.execute(
                """
                INSERT INTO batnav_projet.joueur_humain(id_j, nom, prenom, date_naiss, date_creation_comte)
                VALUES (%s, %s, %s, %s, CURRENT_DATE)
                """,
                [next_id, nom, prenom, date_naissance],
            )
            connexion.commit()
            return next_id
        except psycopg.Error as e:
            connexion.rollback()
            logger.error(e)
            return None


def get_Partie(connexion, id_joueur):
    """
    Retourne les parties d'un joueur.
    """
    query = """
        SELECT *
        FROM batnav_projet.partie p
        JOIN batnav_projet.joueur_humain jh ON p.id_j_humain = jh.id_j
        WHERE p.id_j_humain = %s
        ORDER BY p.id_p DESC
    """
    return execute_select_query(connexion, query, [id_joueur])


def avg_tour(connexion, id_joueur):
    """
    Retourne le nombre moyen de tours par partie.
    """
    query = """
        SELECT AVG(nb_tours)::float AS moyenne_de_tours
        FROM (
            SELECT COUNT(*) AS nb_tours
            FROM batnav_projet.tour t
            JOIN batnav_projet.partie p ON p.id_p = t.id_p
            WHERE p.id_j_humain = %s
            GROUP BY t.id_p
        ) AS sous_requete
    """
    result = execute_one_query(connexion, query, [id_joueur])
    if result and result["moyenne_de_tours"] is not None:
        return float(result["moyenne_de_tours"])
    return None


def nb_tir_carte(connexion, id_joueur):
    """
    Retourne le nombre de tirs par type de carte pour un joueur.
    """
    query = """
        SELECT tc.nom, COUNT(t.id_carte) AS nb_de_tir
        FROM batnav_projet.type_carte tc
        JOIN batnav_projet.carte c ON tc.code_carte = c.code_carte
        JOIN batnav_projet.tir t ON c.id_carte = t.id_carte
        WHERE t.id_j = %s
        GROUP BY tc.nom
        ORDER BY tc.nom
    """
    return execute_select_query(connexion, query, [id_joueur])


def nb_points_2026(connexion, id_joueur):
    """
    Retourne le nombre de parties gagnées contre chaque niveau de joueur virtuel.
    """
    query = """
        SELECT jv.niveau, COUNT(p.id_p) AS partie_gagner
        FROM batnav_projet.partie p
        JOIN batnav_projet.joueur_virtuelle jv ON p.id_j_virtuelle = jv.id_j
        WHERE p.id_j_humain = %s
          AND p.id_j_gagnant = p.id_j_humain
        GROUP BY jv.niveau
        ORDER BY jv.niveau
    """
    return execute_select_query(connexion, query, [id_joueur])


def get_virtual_players(connexion):
    """
    Retourne la liste des adversaires virtuels disponibles.
    """
    query = """
        SELECT
            jv.id_j,
            j.pseudo,
            jv.niveau,
            TO_CHAR(jv.date_creation, 'DD/MM/YYYY') AS date_creation
        FROM batnav_projet.joueur_virtuelle jv
        JOIN batnav_projet.joueur j ON j.id_j = jv.id_j
        ORDER BY
            CASE jv.niveau
                WHEN 'Faible' THEN 1
                WHEN 'Intermédiaire' THEN 2
                WHEN 'Expert' THEN 3
                ELSE 4
            END,
            j.pseudo
    """
    return execute_select_query(connexion, query)


def get_open_games_for_player(connexion, id_joueur):
    """
    Retourne les parties non terminées d'un joueur.
    """
    query = """
        SELECT
            p.id_p,
            TO_CHAR(p.date_creation, 'DD/MM/YYYY') AS date_creation,
            p.heure_creation,
            p.etat,
            jv.niveau,
            j.pseudo AS pseudo_adversaire,
            COALESCE(MAX(t.num_t), 0) AS nb_tours
        FROM batnav_projet.partie p
        JOIN batnav_projet.joueur_virtuelle jv ON jv.id_j = p.id_j_virtuelle
        JOIN batnav_projet.joueur j ON j.id_j = jv.id_j
        LEFT JOIN batnav_projet.tour t ON t.id_p = p.id_p
        WHERE p.id_j_humain = %s
          AND p.etat IN ('Créé', 'En cours', 'Suspendue')
        GROUP BY p.id_p, p.date_creation, p.heure_creation, p.etat, jv.niveau, j.pseudo
        ORDER BY p.id_p DESC
    """
    return execute_select_query(connexion, query, [id_joueur])


def _insert_grille(cursor, id_p, type_grille):
    cursor.execute(
        """
        INSERT INTO batnav_projet.grille(
            id_g, id_p, type_grille, largeur, hauteur, img_non_exp, img_vide, img_touche
        )
        VALUES (
            COALESCE((SELECT MAX(id_g) + 1 FROM batnav_projet.grille), 1),
            %s, %s, 10, 10,
            %s, %s, %s
        )
        RETURNING id_g
        """,
        [
            id_p,
            type_grille,
            "static/img/Cellule_nonExplorée1.png",
            "static/img/Cellule_àLeau.png",
            "static/img/Cellule_touché.png",
        ],
    )
    return cursor.fetchone()[0]


def _insert_navire(cursor, navire, code_pays):
    cursor.execute(
        """
        INSERT INTO batnav_projet.navire(id_n, nom, type_navire, taille, code_pays)
        VALUES (
            COALESCE((SELECT MAX(id_n) + 1 FROM batnav_projet.navire), 1),
            %s, %s, %s, %s
        )
        RETURNING id_n
        """,
        [navire["nom"], navire["type_navire"], navire["taille"], code_pays],
    )
    return cursor.fetchone()[0]


def _create_simple_pioche(cursor):
    cursor.execute(
        """
        INSERT INTO batnav_projet.pioche(id_pi, id_d)
        VALUES (COALESCE((SELECT MAX(id_pi) + 1 FROM batnav_projet.pioche), 1), 1)
        RETURNING id_pi
        """
    )
    id_pi = cursor.fetchone()[0]
    for rang in range(1, 101):
        cursor.execute(
            """
            INSERT INTO batnav_projet.carte(id_carte, rang, id_pi, code_carte)
            VALUES (
                COALESCE((SELECT MAX(id_carte) + 1 FROM batnav_projet.carte), 1),
                %s, %s, 'C_MISSILE'
            )
            """,
            [rang, id_pi],
        )
    return id_pi


def _create_fleet(cursor, id_p, id_g, code_pays, layout):
    for navire in layout:
        id_n = _insert_navire(cursor, navire, code_pays)
        cursor.execute(
            """
            INSERT INTO batnav_projet.est_place(id_p, id_g, id_n, coord_x, coord_y, sens, etat)
            VALUES (%s, %s, %s, %s, %s, %s, 'opérationnel')
            """,
            [id_p, id_g, id_n, navire["coord_x"], navire["coord_y"], navire["sens"]],
        )


def create_simple_game(connexion, id_joueur_humain, id_joueur_virtuel):
    """
    Crée une partie simple pour le jalon 3 phase 1.
    """
    with connexion.cursor() as cursor:
        try:
            id_pi = _create_simple_pioche(cursor)
            cursor.execute(
                """
                INSERT INTO batnav_projet.partie(
                    id_p, date_creation, heure_creation, etat, score_final,
                    id_j_humain, id_j_virtuelle, id_pi, id_j_gagnant
                )
                VALUES (
                    COALESCE((SELECT MAX(id_p) + 1 FROM batnav_projet.partie), 1),
                    CURRENT_DATE,
                    TO_CHAR(CURRENT_TIME, 'HH24:MI:SS'),
                    'Créé',
                    NULL,
                    %s,
                    %s,
                    %s,
                    NULL
                )
                RETURNING id_p
                """,
                [id_joueur_humain, id_joueur_virtuel, id_pi],
            )
            id_p = cursor.fetchone()[0]

            human_ship_grid = _insert_grille(cursor, id_p, 1)
            human_shot_grid = _insert_grille(cursor, id_p, 2)
            ai_ship_grid = _insert_grille(cursor, id_p, 1)
            ai_shot_grid = _insert_grille(cursor, id_p, 2)

            for id_j, id_g in [
                (id_joueur_humain, human_ship_grid),
                (id_joueur_humain, human_shot_grid),
                (id_joueur_virtuel, ai_ship_grid),
                (id_joueur_virtuel, ai_shot_grid),
            ]:
                cursor.execute(
                    "INSERT INTO batnav_projet.dispose_grille(id_j, id_g, id_p) VALUES (%s, %s, %s)",
                    [id_j, id_g, id_p],
                )

            _create_fleet(cursor, id_p, human_ship_grid, "FR", SIMPLE_FLEET_LAYOUT)
            _create_fleet(cursor, id_p, ai_ship_grid, "US", SIMPLE_FLEET_LAYOUT_AI)

            connexion.commit()
            return id_p
        except psycopg.Error as e:
            connexion.rollback()
            logger.error(e)
            return None


def get_game_for_player(connexion, id_p, id_joueur_humain):
    """
    Retourne les informations d'une partie appartenant au joueur humain.
    """
    query = """
        SELECT
            p.*,
            j.pseudo AS pseudo_adversaire,
            jv.niveau
        FROM batnav_projet.partie p
        JOIN batnav_projet.joueur_virtuelle jv ON jv.id_j = p.id_j_virtuelle
        JOIN batnav_projet.joueur j ON j.id_j = jv.id_j
        WHERE p.id_p = %s
          AND p.id_j_humain = %s
    """
    return execute_one_query(connexion, query, [id_p, id_joueur_humain])


def _get_grids_by_player(connexion, id_p):
    rows = execute_select_query(
        connexion,
        """
        SELECT dg.id_j, g.id_g, g.type_grille
        FROM batnav_projet.dispose_grille dg
        JOIN batnav_projet.grille g ON g.id_g = dg.id_g AND g.id_p = dg.id_p
        WHERE dg.id_p = %s
        ORDER BY dg.id_j, g.type_grille, g.id_g
        """,
        [id_p],
    ) or []
    mapping = {}
    for row in rows:
        mapping.setdefault(row["id_j"], {})
        mapping[row["id_j"]][row["type_grille"]] = row["id_g"]
    return mapping


def _letter_to_index(letter):
    return LETTERS.index(letter.upper()) + 1


def _index_to_letter(index):
    return LETTERS[index - 1]


def _ship_cells(ship_row):
    start_x = ship_row["coord_x"]
    start_y = _letter_to_index(ship_row["coord_y"])
    cells = []
    for offset in range(ship_row["taille"]):
        if ship_row["sens"] == "vertical":
            cells.append((start_x + offset, _index_to_letter(start_y)))
        else:
            cells.append((start_x, _index_to_letter(start_y + offset)))
    return cells


def _get_ships_for_grid(connexion, id_p, id_g):
    query = """
        SELECT ep.id_n, ep.coord_x, ep.coord_y, ep.sens, ep.etat, n.nom, n.type_navire, n.taille
        FROM batnav_projet.est_place ep
        JOIN batnav_projet.navire n ON n.id_n = ep.id_n
        WHERE ep.id_p = %s
          AND ep.id_g = %s
        ORDER BY ep.id_n
    """
    return execute_select_query(connexion, query, [id_p, id_g]) or []


def _find_ship_hit(connexion, id_p, id_g, coord_x, coord_y):
    for ship in _get_ships_for_grid(connexion, id_p, id_g):
        if (coord_x, coord_y) in _ship_cells(ship):
            return ship
    return None


def _revealed_cells_for_grid(connexion, id_g):
    rows = execute_select_query(
        connexion,
        """
        SELECT coord_x, coord_y
        FROM batnav_projet.cellule_devoile
        WHERE id_g = %s
        """,
        [id_g],
    ) or []
    return {(row["coord_x"], row["coord_y"]) for row in rows}


def _already_targeted(connexion, id_p, id_j, coord_x, coord_y):
    row = execute_one_query(
        connexion,
        """
        SELECT 1
        FROM batnav_projet.tir
        WHERE id_p = %s
          AND id_j = %s
          AND coord_x = %s
          AND coord_y = %s
        LIMIT 1
        """,
        [id_p, id_j, coord_x, coord_y],
    )
    return row is not None


def _draw_next_card(connexion, id_p):
    query = """
        SELECT c.id_carte, c.code_carte, tc.nom, tc.description, tc.img
        FROM batnav_projet.partie p
        JOIN batnav_projet.carte c ON c.id_pi = p.id_pi
        JOIN batnav_projet.type_carte tc ON tc.code_carte = c.code_carte
        LEFT JOIN batnav_projet.tir t ON t.id_carte = c.id_carte AND t.id_p = p.id_p
        WHERE p.id_p = %s
          AND t.id_carte IS NULL
        ORDER BY c.rang
        LIMIT 1
    """
    return execute_one_query(connexion, query, [id_p])


def get_next_card_preview(connexion, id_p):
    """
    Retourne la prochaine carte de la pioche.
    """
    return _draw_next_card(connexion, id_p)


def _get_current_turn_number(connexion, id_p):
    row = execute_one_query(
        connexion,
        "SELECT COALESCE(MAX(num_t), 0) AS num_t FROM batnav_projet.tour WHERE id_p = %s",
        [id_p],
    )
    return row["num_t"] if row else 0


def _get_turn_shot_count(connexion, id_p, num_t):
    row = execute_one_query(
        connexion,
        """
        SELECT COUNT(*) AS nb
        FROM batnav_projet.tir
        WHERE id_p = %s
          AND num_t = %s
        """,
        [id_p, num_t],
    )
    return row["nb"] if row else 0


def start_game(connexion, id_p, id_joueur_humain):
    """
    Lance la partie et crée le premier tour.
    """
    partie = get_game_for_player(connexion, id_p, id_joueur_humain)
    if partie is None:
        return False
    if _get_current_turn_number(connexion, id_p) > 0:
        return True
    with connexion.cursor() as cursor:
        try:
            cursor.execute(
                """
                UPDATE batnav_projet.partie
                SET etat = 'En cours'
                WHERE id_p = %s
                """,
                [id_p],
            )
            cursor.execute(
                """
                INSERT INTO batnav_projet.tour(
                    id_p, num_t, nb_navire_coule_j, nb_cellule_touche_j, nb_cellule_exp_j,
                    nb_navire_coule_a, nb_cellule_touche_a, nb_cellule_exp_a
                )
                VALUES (%s, 1, 0, 0, 0, 0, 0, 0)
                """,
                [id_p],
            )
            connexion.commit()
            return True
        except psycopg.Error as e:
            connexion.rollback()
            logger.error(e)
            return False


def advance_to_next_turn(connexion, id_p, id_joueur_humain):
    """
    Passe au tour suivant si la partie est toujours en cours.
    """
    partie = get_game_for_player(connexion, id_p, id_joueur_humain)
    if partie is None or partie["etat"] not in ("En cours", "Créé", "Suspendue"):
        return False
    current_turn = _get_current_turn_number(connexion, id_p)
    if current_turn == 0 or _get_turn_shot_count(connexion, id_p, current_turn) < 2:
        return False
    with connexion.cursor() as cursor:
        try:
            cursor.execute(
                """
                INSERT INTO batnav_projet.tour(
                    id_p, num_t, nb_navire_coule_j, nb_cellule_touche_j, nb_cellule_exp_j,
                    nb_navire_coule_a, nb_cellule_touche_a, nb_cellule_exp_a
                )
                VALUES (%s, %s, 0, 0, 0, 0, 0, 0)
                """,
                [id_p, current_turn + 1],
            )
            connexion.commit()
            return True
        except psycopg.Error as e:
            connexion.rollback()
            logger.error(e)
            return False


def _set_ship_state(connexion, id_p, id_g, ship):
    revealed = _revealed_cells_for_grid(connexion, id_g)
    cells = _ship_cells(ship)
    hits = sum(1 for cell in cells if cell in revealed)
    state = "opérationnel"
    if hits == len(cells):
        state = "coulé"
    elif hits > 0:
        state = "touché"
    with connexion.cursor() as cursor:
        cursor.execute(
            """
            UPDATE batnav_projet.est_place
            SET etat = %s
            WHERE id_p = %s
              AND id_g = %s
              AND id_n = %s
            """,
            [state, id_p, id_g, ship["id_n"]],
        )
    return state


def _all_ships_sunk(connexion, id_p, id_g):
    ships = _get_ships_for_grid(connexion, id_p, id_g)
    if not ships:
        return False
    for ship in ships:
        if _set_ship_state(connexion, id_p, id_g, ship) != "coulé":
            return False
    return True


def _board_stats(connexion, id_p, id_g):
    ships = _get_ships_for_grid(connexion, id_p, id_g)
    revealed = _revealed_cells_for_grid(connexion, id_g)
    coules = 0
    touches = 0
    explores = len(revealed)
    for ship in ships:
        cells = _ship_cells(ship)
        hits = sum(1 for cell in cells if cell in revealed)
        if hits == len(cells):
            coules += 1
        if hits > 0:
            touches += hits
    return {
        "nb_navire_coule": coules,
        "nb_cellule_touche": touches,
        "nb_cellule_exp": explores,
    }


def _update_turn_stats(connexion, id_p, num_t, human_ship_grid, ai_ship_grid):
    player_stats = _board_stats(connexion, id_p, ai_ship_grid)
    ai_stats = _board_stats(connexion, id_p, human_ship_grid)
    with connexion.cursor() as cursor:
        cursor.execute(
            """
            UPDATE batnav_projet.tour
            SET nb_navire_coule_j = %s,
                nb_cellule_touche_j = %s,
                nb_cellule_exp_j = %s,
                nb_navire_coule_a = %s,
                nb_cellule_touche_a = %s,
                nb_cellule_exp_a = %s
            WHERE id_p = %s
              AND num_t = %s
            """,
            [
                player_stats["nb_navire_coule"],
                player_stats["nb_cellule_touche"],
                player_stats["nb_cellule_exp"],
                ai_stats["nb_navire_coule"],
                ai_stats["nb_cellule_touche"],
                ai_stats["nb_cellule_exp"],
                id_p,
                num_t,
            ],
        )


def _finish_game(connexion, id_p, winner_id):
    total_tirs = execute_one_query(
        connexion,
        "SELECT COUNT(*) AS nb FROM batnav_projet.tir WHERE id_p = %s",
        [id_p],
    )["nb"]
    score = math.floor((100 * 17) / max(total_tirs, 1))
    with connexion.cursor() as cursor:
        cursor.execute(
            """
            UPDATE batnav_projet.partie
            SET etat = CASE
                WHEN id_j_humain = %s THEN 'Gagnée'
                ELSE 'Perdue'
            END,
                id_j_gagnant = %s,
                score_final = %s
            WHERE id_p = %s
            """,
            [winner_id, winner_id, score, id_p],
        )


def _choose_random_target(connexion, id_p, id_joueur):
    remaining = [
        (x, y)
        for x in range(1, 11)
        for y in LETTERS
        if not _already_targeted(connexion, id_p, id_joueur, x, y)
    ]
    if not remaining:
        return None
    return random.choice(remaining)


def _insert_tir_and_reveal(cursor, id_p, num_t, num_ti, coord_x, coord_y, id_j, id_carte, target_grid):
    cursor.execute(
        """
        INSERT INTO batnav_projet.tir(id_p, num_t, num_ti, coord_x, coord_y, id_j, id_carte)
        VALUES (%s, %s, %s, %s, %s, %s, %s)
        """,
        [id_p, num_t, num_ti, coord_x, coord_y, id_j, id_carte],
    )
    cursor.execute(
        """
        INSERT INTO batnav_projet.cellule_devoile(id_p, num_t, num_ti, id_g, coord_x, coord_y)
        VALUES (%s, %s, %s, %s, %s, %s)
        """,
        [id_p, num_t, num_ti, target_grid, coord_x, coord_y],
    )


def play_turn(connexion, id_p, id_joueur_humain, coord_x, coord_y):
    """
    Exécute un tour complet simple : tir du joueur puis tir aléatoire de l'IA.
    """
    partie = get_game_for_player(connexion, id_p, id_joueur_humain)
    if partie is None or partie["etat"] not in ("En cours",):
        return {"ok": False, "message": "La partie n'est pas jouable actuellement."}

    current_turn = _get_current_turn_number(connexion, id_p)
    if current_turn == 0:
        return {"ok": False, "message": "La partie n'a pas encore commencé."}
    if _get_turn_shot_count(connexion, id_p, current_turn) != 0:
        return {"ok": False, "message": "Ce tour est déjà terminé. Passez au suivant."}
    if _already_targeted(connexion, id_p, id_joueur_humain, coord_x, coord_y):
        return {"ok": False, "message": "Cette case a déjà été ciblée."}

    grids = _get_grids_by_player(connexion, id_p)
    human_grids = grids.get(id_joueur_humain, {})
    ai_grids = grids.get(partie["id_j_virtuelle"], {})
    human_ship_grid = human_grids.get(1)
    ai_ship_grid = ai_grids.get(1)
    if not human_ship_grid or not ai_ship_grid:
        return {"ok": False, "message": "Les grilles de la partie sont incomplètes."}

    player_card = _draw_next_card(connexion, id_p)
    if player_card is None:
        return {"ok": False, "message": "La pioche est vide."}

    player_hit_ship = _find_ship_hit(connexion, id_p, ai_ship_grid, coord_x, coord_y)
    player_hit = player_hit_ship is not None

    ai_target = _choose_random_target(connexion, id_p, partie["id_j_virtuelle"])
    if ai_target is None:
        return {"ok": False, "message": "Aucune case disponible pour l'adversaire."}

    with connexion.cursor() as cursor:
        try:
            _insert_tir_and_reveal(
                cursor,
                id_p,
                current_turn,
                1,
                coord_x,
                coord_y,
                id_joueur_humain,
                player_card["id_carte"],
                ai_ship_grid,
            )
            player_ship_state = None
            if player_hit_ship is not None:
                player_ship_state = _set_ship_state(connexion, id_p, ai_ship_grid, player_hit_ship)

            player_won = _all_ships_sunk(connexion, id_p, ai_ship_grid)
            ai_hit_ship = None
            ai_hit = False
            ai_ship_state = None
            ai_card = None

            if not player_won:
                ai_card = _draw_next_card(connexion, id_p)
                if ai_card is None:
                    raise RuntimeError("Pioche vide pour l'adversaire")
                ai_coord_x, ai_coord_y = ai_target
                ai_hit_ship = _find_ship_hit(connexion, id_p, human_ship_grid, ai_coord_x, ai_coord_y)
                ai_hit = ai_hit_ship is not None
                _insert_tir_and_reveal(
                    cursor,
                    id_p,
                    current_turn,
                    2,
                    ai_coord_x,
                    ai_coord_y,
                    partie["id_j_virtuelle"],
                    ai_card["id_carte"],
                    human_ship_grid,
                )
                if ai_hit_ship is not None:
                    ai_ship_state = _set_ship_state(connexion, id_p, human_ship_grid, ai_hit_ship)
            else:
                ai_coord_x, ai_coord_y = None, None

            ai_won = False
            if not player_won:
                ai_won = _all_ships_sunk(connexion, id_p, human_ship_grid)

            _update_turn_stats(connexion, id_p, current_turn, human_ship_grid, ai_ship_grid)

            if player_won:
                _finish_game(connexion, id_p, id_joueur_humain)
            elif ai_won:
                _finish_game(connexion, id_p, partie["id_j_virtuelle"])

            connexion.commit()

            result = {
                "ok": True,
                "player_card": player_card,
                "player_shot": {"coord_x": coord_x, "coord_y": coord_y, "hit": player_hit, "ship_state": player_ship_state},
                "ai_card": ai_card if not player_won else None,
                "ai_shot": None if player_won else {
                    "coord_x": ai_coord_x,
                    "coord_y": ai_coord_y,
                    "hit": ai_hit,
                    "ship_state": ai_ship_state,
                },
                "finished": player_won or ai_won,
                "winner": id_joueur_humain if player_won else (partie["id_j_virtuelle"] if ai_won else None),
            }
            return result
        except Exception as e:
            connexion.rollback()
            logger.error(e)
            return {"ok": False, "message": "Erreur SQL pendant le tour."}


def get_turn_log(connexion, id_p, num_t):
    """
    Retourne les tirs d'un tour avec les informations de carte.
    """
    query = """
        SELECT
            t.num_ti,
            t.coord_x,
            t.coord_y,
            t.id_j,
            tc.nom AS nom_carte,
            tc.code_carte,
            tc.img
        FROM batnav_projet.tir t
        JOIN batnav_projet.carte c ON c.id_carte = t.id_carte
        JOIN batnav_projet.type_carte tc ON tc.code_carte = c.code_carte
        WHERE t.id_p = %s
          AND t.num_t = %s
        ORDER BY t.num_ti
    """
    return execute_select_query(connexion, query, [id_p, num_t]) or []


def _grid_cells_with_images(grid_matrix, image_map):
    rows = []
    for row in grid_matrix:
        rows.append(
            [
                {
                    "state": cell,
                    "img": image_map.get(cell, image_map["?"]),
                }
                for cell in row
            ]
        )
    return rows


def _build_player_ship_grid(connexion, id_p, ship_grid):
    grid = {(x, y): "?" for x in range(1, 11) for y in LETTERS}
    for ship in _get_ships_for_grid(connexion, id_p, ship_grid):
        for cell in _ship_cells(ship):
            grid[cell] = "N"
    revealed = _revealed_cells_for_grid(connexion, ship_grid)
    for coord in revealed:
        grid[coord] = "T" if grid[coord] == "N" else "P"
    return [[grid[(x, y)] for y in LETTERS] for x in range(1, 11)]


def _build_player_target_grid(connexion, id_p, human_id, ai_ship_grid):
    grid = {(x, y): "?" for x in range(1, 11) for y in LETTERS}
    tirs = execute_select_query(
        connexion,
        """
        SELECT coord_x, coord_y
        FROM batnav_projet.tir
        WHERE id_p = %s
          AND id_j = %s
          AND coord_x IS NOT NULL
          AND coord_y IS NOT NULL
        """,
        [id_p, human_id],
    ) or []
    for tir in tirs:
        ship = _find_ship_hit(connexion, id_p, ai_ship_grid, tir["coord_x"], tir["coord_y"])
        grid[(tir["coord_x"], tir["coord_y"])] = "T" if ship else "P"
    return [[grid[(x, y)] for y in LETTERS] for x in range(1, 11)]


def get_game_view(connexion, id_p, id_joueur_humain):
    """
    Prépare les données d'affichage de la page de jeu.
    """
    partie = get_game_for_player(connexion, id_p, id_joueur_humain)
    if partie is None:
        return None
    grids = _get_grids_by_player(connexion, id_p)
    human_grids = grids.get(id_joueur_humain, {})
    ai_grids = grids.get(partie["id_j_virtuelle"], {})
    human_ship_grid = human_grids.get(1)
    ai_ship_grid = ai_grids.get(1)
    current_turn = _get_current_turn_number(connexion, id_p)
    turn_log = get_turn_log(connexion, id_p, current_turn) if current_turn else []
    nb_tirs_tour = _get_turn_shot_count(connexion, id_p, current_turn) if current_turn else 0
    grille_navires = _build_player_ship_grid(connexion, id_p, human_ship_grid) if human_ship_grid else []
    grille_tirs = _build_player_target_grid(connexion, id_p, id_joueur_humain, ai_ship_grid) if ai_ship_grid else []
    return {
        "partie": partie,
        "tour_courant": current_turn,
        "nb_tirs_tour": nb_tirs_tour,
        "grille_navires": _grid_cells_with_images(grille_navires, GRID_IMAGE_MAP_SHIP),
        "grille_tirs": _grid_cells_with_images(grille_tirs, GRID_IMAGE_MAP_SHOT),
        "images_grille_navires": GRID_IMAGE_MAP_SHIP,
        "images_grille_tirs": GRID_IMAGE_MAP_SHOT,
        "carte_courante": get_next_card_preview(connexion, id_p) if partie["etat"] == "En cours" and current_turn and nb_tirs_tour == 0 else None,
        "journal_tour": turn_log,
    }
