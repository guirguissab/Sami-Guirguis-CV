from controleurs.includes import add_activity
from model.model_pg import get_Partie,avg_tour,nb_tir_carte,nb_points_2026

REQUEST_VARS['page_id'] = 'accueil'

add_activity(SESSION['HISTORIQUE'], "consultation de la page accueil")

joueur = SESSION.get('JOUEUR_COURANT')
if joueur is None:
    REQUEST_VARS['message'] = "Aucun joueur connecté. Utilisez la page de connexion."
    REQUEST_VARS['message_class'] = "alert-warning"
else:
    REQUEST_VARS['joueur'] = joueur
    if joueur and POST :
        if 'chercher_partie' in POST:
            term = joueur['id_j']
            result = get_Partie(SESSION['CONNEXION'], term)
            if not result:
                REQUEST_VARS['message'] = f"Aucune partie trouvé pour '{joueur['pseudo']}'."
                REQUEST_VARS['message_class'] = "alert-error"
            else:
                REQUEST_VARS['message_info'] = f"{len(result)} partie jouer par: '{joueur['pseudo']}'."
                REQUEST_VARS['result'] = result
                REQUEST_VARS['message_class'] = "alert-success"
        elif 'chercher_avg' in POST:
            term = joueur['id_j']
            result = get_Partie(SESSION['CONNEXION'], term)
            if not result:
                REQUEST_VARS['message'] = f"Aucune partie jouer par '{joueur['pseudo']}'."
                REQUEST_VARS['message_class'] = "alert-error"
            else:
                average=avg_tour(SESSION['CONNEXION'], term)
                REQUEST_VARS['message_info'] = f"{average} moyenne de tour dans les parties de '{joueur['pseudo']}'."
                REQUEST_VARS['average'] = average
                REQUEST_VARS['message_class'] = "alert-success"
        elif 'chercher_nb_carte' in POST:
            term=joueur['id_j']
            result = get_Partie(SESSION['CONNEXION'], term)
            if not result:
                REQUEST_VARS['message'] = f"Aucune partie jouer par '{joueur['pseudo']}'."
                REQUEST_VARS['message_class'] = "alert-error"
            else:
                nb_carte=nb_tir_carte(SESSION['CONNEXION'], term)
                REQUEST_VARS['message_info'] = f"ceci Les cartes tirer et leur reccurence par '{joueur['pseudo']}'."
                REQUEST_VARS['nb_carte'] = nb_carte
                REQUEST_VARS['message_class'] = "alert-success"
        elif 'chercher_partie_gagn' in POST:
            term=joueur['id_j']
            result = get_Partie(SESSION['CONNEXION'], term)
            if not result:
                REQUEST_VARS['message'] = f"Aucune partie jouer par '{joueur['pseudo']}'."
                REQUEST_VARS['message_class'] = "alert-error"
            else :
                chercher_gagn=nb_points_2026(SESSION['CONNEXION'], term)
                REQUEST_VARS['message_info'] = f"Le nombre de partie gagner contre chaque'{joueur['pseudo']}'."
                REQUEST_VARS['chercher_gagn'] = chercher_gagn
                REQUEST_VARS['message_class'] = "alert-success"





