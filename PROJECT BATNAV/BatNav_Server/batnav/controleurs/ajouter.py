from model.model_pg import insert_human_player, get_human_player_by_id
from controleurs.includes import add_activity
from datetime import datetime

REQUEST_VARS['page_id'] = 'inscription'
add_activity(SESSION['HISTORIQUE'], "consultation de la page inscription")

if POST and 'bouton_inscription' in POST:
    nom = POST['nom'][0].strip()
    prenom = POST['prenom'][0].strip()
    date_naissance = POST['date_naissance'][0].strip()
    pseudo = POST['pseudo'][0].strip()

    if not nom or not prenom or not date_naissance or not pseudo:
        REQUEST_VARS['message'] = "Tous les champs sont obligatoires."
        REQUEST_VARS['message_class'] = "alert-warning"
    else:
        try:
            date_naissance = datetime.strptime(date_naissance, "%d/%m/%Y").strftime("%Y-%m-%d")
        except ValueError:
            REQUEST_VARS['message'] = "Date invalide. Utilisez le format JJ/MM/AAAA."
            REQUEST_VARS['message_class'] = "alert-error"
            date_naissance = None

        if not REQUEST_VARS.get('message'):
            id_joueur = insert_human_player(
                SESSION['CONNEXION'],
                pseudo,
                nom,
                prenom,
                date_naissance,
            )
            if id_joueur is not None:
                joueur = get_human_player_by_id(SESSION['CONNEXION'], id_joueur)
                SESSION['JOUEUR_COURANT'] = joueur
                REQUEST_VARS['message'] = f"Compte créé avec succès. Bienvenue {pseudo}."
                REQUEST_VARS['message_class'] = "alert-success"
            else:
                REQUEST_VARS['message'] = "Erreur lors de la création du compte."
                REQUEST_VARS['message_class'] = "alert-error"