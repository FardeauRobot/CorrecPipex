# CHANGES

## 1. Project Structure Refactor

### Before (`nvpipex`)
- Main files were spread at root level:
- `main.c`, `pipex.h`, `parsing/`, `free/`, `exec/`, `ft_print/`

### After (`nvpipextim`)
- Code is organized by role:
- `src/` contains implementation files (`main`, `parsing`, `exec`, `utils`)
- `includes/` contains headers and local dependency headers (`pipex.h`, `ft_print/`)

### Why this is better
- Clearer separation between headers and implementation.
- Easier to navigate when the project grows.
- Helps maintain a clean architecture (sources vs interfaces).

## 2. Header Location and Content Updates

### Location change
- `pipex.h` moved from project root to `includes/pipex.h`.

### API/struct changes
- In `t_cmd`:
- `cmd` was renamed to `args`
- `fdinput` and `fdoutput` changed from `char` to `int`

### Why this is better
- `args` is clearer for future `execve` usage.
- File descriptors are integers in POSIX (`open`, `dup2`, `close`), so `int` is the correct type.

## 3. Parsing Uses the New Command Field

### What changed
- In parsing logic, assignments switched from:
- `pipex->cmdX.cmd`
- to:
- `pipex->cmdX.args`

### Why this is better
- Keeps the code aligned with the updated struct field names.
- Reduces confusion when connecting parsing to execution later.

## 4. Debug Output Is Now Centralized

### Before
- `main.c` printed each part of the struct manually with repeated loops.

### After
- Dedicated helper functions are used:
- `ft_arg_print`
- `ft_cmd_print`
- `ft_matrix_print`

### Why this is better
- Less duplicated code.
- Easier to change formatting in one place.
- `main.c` stays shorter and easier to read.

## 5. Utility Code Grouped in `src/utils`

### What changed
- `free.c` was moved into `src/utils/free.c`.
- `output.c` was added in `src/utils/output.c`.
- `utils.c` exists as a placeholder for future helpers.

### Why this is better
- Utility behavior is grouped together.
- Core logic files remain focused on parsing/execution flow.

## 6. Makefile Modernization

### What changed
- Source files are grouped by subdirectory variables.
- `addprefix` is used to compose full source paths.
- Object files are generated into `objs/`.
- Dependency files (`.d`) are auto-generated with `-MMD -MP`.
- `clean` removes `objs/`.
- `fclean` removes `objs/` and the final binary.

### Why this is better
- Build artifacts no longer pollute source directories.
- Header changes trigger proper incremental rebuilds.
- Cleanup behavior is predictable and complete.

## 7. Behavior Status (Current State)

### Implemented now
- Parsing and path lookup scaffolding are present.
- Debug inspection output is available.
- Build system is ready for incremental development.

### Still to complete
- Real execution pipeline behavior in `exec` (fork/pipe/dup2/execve/waitpid).
- Full error handling and memory/file-descriptor cleanup paths.

This transition is mostly a **codebase organization upgrade** plus a few **important type/field fixes**.
The project is now in a better state to continue implementing the real `pipex` execution flow safely and clearly.

## BRAVOOOOO

T'as fait le plus gros au niveau du parsing.
Il va manquer l'initialisation du path pour tes commandes, potentiellement revoir ta fonction isolatepath pour éviter d'avoir 2 splits a l'intérieur.

Petite explication pour mon parsing :

	On utilise le machine state

	C'est le vrai fonctionnement de comment le shell traite les lignes qu'on lui donne en entrée . 
	Plutot que de diviser mot par mot en fonction de séparateur, il va traiter la ligne caractere par caractere en fonction de l'état dans lequel il est, et qui sera lui meme défini par les caracteres qui le font changer d'état comme les espaces, " ou ', pour savoir quand s'arrete et quand commence le nouveau "mot".

	Par exemple : 
		ec"ho" coucou est considéré comme : echo coucou

	C'est un peu la les limites d'utiliser split. Certains utilisent un split modifié avec des flags, mais pour comprendre la suite du fonctionnement du shell ca devient plus hard. Dans tous les cas, et c'est ce que m'ont dit certaines personnes quand je les corrigeais, te prends pas la tete a faire un parsing parfait pour pipex, la vraie valeur du projet est sur comprendre l'exécution via les redirections ( |, < ou > ) et les forks

Comme dit aussi j'ai du changer ton printf pour gérer certains cas ou j'imprimais plusieurs variables dans le meme ft_printf(). J'ai l'impression que les pointeurs et leur utilisation ont encore quelques petits secrets a te partager. 

L'exec reste a faire mais pour le moment ca me parait good. Encore une fois si t'as des galeres ou que t'as des questions, hésite pas. CA VA LE FAIRE 💪
