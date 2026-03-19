# Minishell

Un shell minimal en C inspiré de Bash, réalisé dans l’esprit du projet `minishell`.

L’objectif : reproduire le comportement d’un shell Unix sur un sous-ensemble défini,
avec une attention particulière sur le parsing, l’exécution des commandes et la gestion
des signaux/retours d’erreur.

---

## Sommaire

1. [À propos](#à-propos)
2. [Fonctionnalités](#fonctionnalités)
3. [Architecture du projet](#architecture-du-projet)
4. [Compilation](#compilation)
5. [Utilisation](#utilisation)
6. [Comportements shell pris en charge](#comportements-shell-pris-en-charge)
7. [Gestion des erreurs et signaux](#gestion-des-erreurs-et-signaux)
8. [Jeu de tests conseillé](#jeu-de-tests-conseillé)
9. [Debug & outils utiles](#debug--outils-utiles)
10. [Limitations connues](#limitations-connues)
11. [Roadmap](#roadmap)
12. [Auteurs](#auteurs)

---

## À propos

`minishell` est un mini-interpréteur de commandes interactif.

Le projet couvre généralement :

- boucle REPL (`read -> parse -> execute`),
- exécution de binaires via `PATH`,
- gestion des builtins,
- pipes et redirections,
- variables d’environnement,
- gestion des signaux en mode interactif.

Ce dépôt sert de base de travail et de documentation technique pour développer,
maintenir et tester proprement le shell.

---

## Fonctionnalités

### Prompt & lecture

- Prompt interactif.
- Historique de commandes (si `readline` est utilisé).
- Gestion de `Ctrl-C`, `Ctrl-D`, `Ctrl-\` selon le contexte.

### Parsing

- Tokenisation des commandes.
- Gestion des quotes simples `'...'` et doubles `"..."`.
- Expansion des variables d’environnement (`$USER`, `$HOME`, `$?`, etc.).
- Construction d’une représentation exploitable de la ligne de commande (liste/AST).

### Exécution

- Commandes simples.
- Chaînage de commandes par pipelines (`|`).
- Redirections d’entrée/sortie (`<`, `>`, `>>`, `<<` selon implémentation).
- Résolution des exécutables via `PATH`.
- Code de retour correctement propagé (`$?`).

### Builtins (classiques minishell)

- `echo` (avec gestion de `-n` selon votre spec)
- `cd`
- `pwd`
- `export`
- `unset`
- `env`
- `exit`

---

## Architecture du projet

> Adapte cette section à votre arborescence finale si besoin.

Architecture recommandée :

```text
.
├── Makefile
├── README.md
├── include/
│   ├── minishell.h
│   └── ...
├── src/
│   ├── main.c
│   ├── init/
│   ├── lexer/
│   ├── parser/
│   ├── expander/
│   ├── executor/
│   ├── builtins/
│   ├── signals/
│   ├── env/
│   └── utils/
└── libft/
```

### Flux global

1. Lire la ligne utilisateur.
2. Vérifier la syntaxe de base.
3. Tokeniser.
4. Étendre les variables.
5. Construire la structure de commande (liste/AST).
6. Exécuter (builtin ou binaire externe).
7. Mettre à jour le statut global (`$?`) et relancer la boucle.

---

## Compilation

Commandes classiques :

```bash
make
make clean
make fclean
make re
```

Lancement :

```bash
./minishell
```

Si le projet dépend de `readline`, installez la librairie adaptée à votre système
avant compilation.

---

## Utilisation

Exemples rapides :

```bash
minishell$ pwd
minishell$ echo "Salut $USER"
minishell$ ls -la | grep minishell
minishell$ export TEST_VAR=42
minishell$ echo $TEST_VAR
minishell$ cat < infile | wc -l > outfile
minishell$ exit
```

Exemples de builtins :

```bash
minishell$ env
minishell$ cd /tmp
minishell$ pwd
minishell$ unset TEST_VAR
```

---

## Comportements shell pris en charge

Selon l’avancement de votre implémentation, documente clairement ce qui est **OK**,
**partiel**, ou **non implémenté**.

Checklist type :

- [x] Exécution commande simple
- [x] Pipes
- [x] Redirections `>`, `>>`, `<`
- [ ] Heredoc `<<`
- [x] Variables d’environnement
- [x] `$?`
- [x] Builtins obligatoires
- [ ] Parenthèses / priorités avancées
- [ ] Opérateurs logiques `&&` / `||`
- [ ] Wildcards (`*`)

---

## Gestion des erreurs et signaux

### Erreurs usuelles

- Commande introuvable.
- Permission refusée.
- Fichier/redirection invalide.
- Erreur de syntaxe (`|` mal placé, quote non fermée, etc.).

### Signaux (interactif)

- `Ctrl-C` : interrompt la commande en cours et rend la main au prompt.
- `Ctrl-D` : quitte proprement le shell si ligne vide.
- `Ctrl-\` : comportement aligné avec la spec du projet.

Bonne pratique : centraliser la gestion des signaux selon le contexte
(prompt, enfant en exécution, heredoc).

---

## Jeu de tests conseillé

Créez un jeu de tests reproductible :

### 1) Tests manuels de base

- `echo`, `cd`, `pwd`, `env`, `export`, `unset`, `exit`
- commandes avec et sans arguments
- quotes simples/doubles
- variables (`$USER`, `$?`)

### 2) Pipes & redirections

- `ls | wc -l`
- `cat < infile`
- `echo test > out && cat out` (si `&&` non supporté, en 2 commandes)
- append `>>`

### 3) Cas limites

- ligne vide
- espaces multiples
- quotes non fermées
- redirection sans cible
- pipeline incomplet

### 4) Robustesse mémoire

Tester avec `valgrind` (ou équivalent) :

```bash
valgrind --leak-check=full --show-leak-kinds=all ./minishell
```

Objectif : pas de fuite sur exécutions normales et erreurs de parsing.

---

## Debug & outils utiles

- `gdb` / `lldb` pour le debug pas-à-pas.
- `valgrind` pour les leaks et invalid reads/writes.
- logs temporaires ciblés (lexer, parser, executor) puis retrait avant rendu.

Exemple de session debug :

```bash
gdb ./minishell
(gdb) run
```

---

## Limitations connues

Exemples de limitations à maintenir à jour :

- expansion complexe non 100% Bash-compatible,
- gestion partielle de certains cas de quotes imbriquées,
- comportements non couverts hors périmètre du sujet.

Documenter les limites évite les ambiguïtés au moment de l’évaluation.

---

## Roadmap

- Stabiliser le parsing des cas limites.
- Fiabiliser la gestion des signaux en heredoc.
- Harmoniser les messages d’erreur avec Bash.
- Ajouter un script de tests automatisés.
- Améliorer la documentation des modules.

---

## Auteurs

- `Auxence`
- `hbray`

---

## Licence

Projet académique. Adapter selon votre contexte (école/promo/équipe).
