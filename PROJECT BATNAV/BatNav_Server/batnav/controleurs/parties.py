from controleurs.includes import add_activity
from model.model_pg import create_simple_game, get_open_games_for_player, get_virtual_players


REQUEST_VARS["page_id"] = "parties"
add_activity(SESSION["HISTORIQUE"], "consultation de la page des parties")

joueur = SESSION.get("JOUEUR_COURANT")
if joueur is None:
    REQUEST_VARS["message"] = "Aucun joueur connecté. Utilisez la page de connexion."
    REQUEST_VARS["message_class"] = "alert-warning"
else:
    REQUEST_VARS["joueur"] = joueur

    if POST and "creer_partie" in POST:
        id_adversaire = POST.get("id_adversaire", [""])[0]
        if not id_adversaire:
            REQUEST_VARS["message"] = "Sélectionnez un adversaire virtuel."
            REQUEST_VARS["message_class"] = "alert-warning"
        else:
            id_partie = create_simple_game(SESSION["CONNEXION"], joueur["id_j"], int(id_adversaire))
            if id_partie is None:
                REQUEST_VARS["message"] = "Impossible de créer la partie."
                REQUEST_VARS["message_class"] = "alert-error"
            else:
                SESSION["ID_PARTIE_COURANTE"] = id_partie
                REQUEST_VARS["redirect_jeu"] = True

    if POST and "reprendre_partie" in POST:
        id_partie = POST.get("id_partie", [""])[0]
        if id_partie:
            SESSION["ID_PARTIE_COURANTE"] = int(id_partie)
            REQUEST_VARS["redirect_jeu"] = True

    REQUEST_VARS["parties_en_cours"] = get_open_games_for_player(SESSION["CONNEXION"], joueur["id_j"]) or []
    REQUEST_VARS["adversaires"] = get_virtual_players(SESSION["CONNEXION"]) or []
