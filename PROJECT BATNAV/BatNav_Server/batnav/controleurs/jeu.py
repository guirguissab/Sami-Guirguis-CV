from controleurs.includes import add_activity
from model.model_pg import advance_to_next_turn, get_game_view, play_turn, start_game,_get_ships_for_grid


REQUEST_VARS["page_id"] = "jeu"
add_activity(SESSION["HISTORIQUE"], "consultation de la page de jeu")

joueur = SESSION.get("JOUEUR_COURANT")
id_partie = SESSION.get("ID_PARTIE_COURANTE")

if joueur is None:
    REQUEST_VARS["message"] = "Aucun joueur connecté. Utilisez la page de connexion."
    REQUEST_VARS["message_class"] = "alert-warning"
elif id_partie is None:
    REQUEST_VARS["message"] = "Aucune partie sélectionnée. Passez par la page des parties."
    REQUEST_VARS["message_class"] = "alert-warning"
else:
    REQUEST_VARS["joueur"] = joueur

    if POST and "commencer_partie" in POST:
        if start_game(SESSION["CONNEXION"], id_partie, joueur["id_j"]):
            REQUEST_VARS["message"] = "La partie est lancée. À vous de tirer."
            REQUEST_VARS["message_class"] = "alert-success"
        else:
            REQUEST_VARS["message"] = "Impossible de démarrer la partie."
            REQUEST_VARS["message_class"] = "alert-error"

    if POST and "tirer" in POST:
        coord_x = POST.get("coord_x", [""])[0]
        coord_y = POST.get("coord_y", [""])[0]
        if not coord_x or not coord_y:
            REQUEST_VARS["message"] = "Sélectionnez une ligne et une colonne."
            REQUEST_VARS["message_class"] = "alert-warning"
        else:
            result = play_turn(SESSION["CONNEXION"], id_partie, joueur["id_j"], int(coord_x), coord_y)
            if not result["ok"]:
                REQUEST_VARS["message"] = result["message"]
                REQUEST_VARS["message_class"] = "alert-error"
            else:
                REQUEST_VARS["tour_resultat"] = result
                REQUEST_VARS["message"] = "Le tour a été joué et enregistré en base."
                REQUEST_VARS["message_class"] = "alert-success"

    if POST and "suivant" in POST:
        if advance_to_next_turn(SESSION["CONNEXION"], id_partie, joueur["id_j"]):
            REQUEST_VARS["message"] = "Tour suivant prêt."
            REQUEST_VARS["message_class"] = "alert-success"
        else:
            REQUEST_VARS["message"] = "Impossible de passer au tour suivant."
            REQUEST_VARS["message_class"] = "alert-warning"

    jeu = get_game_view(SESSION["CONNEXION"], id_partie, joueur["id_j"])
    if jeu is None:
        REQUEST_VARS["message"] = "La partie demandée est introuvable."
        REQUEST_VARS["message_class"] = "alert-error"
    else:
        REQUEST_VARS["jeu"] = jeu
