This page describes rules to contribute changes and features by Pull Requests creating.

## Initialize

To initialize your repo do:

* Fork `https://github.com/ostis-ai/ostis-metasystem`.
* Clone your fork to your machine and prepare (see [Readme](https://github.com/ostis-ai/ostis-metasystem)).

```sh
git clone git@github.com:yourlogin/ostis-metasystem.git
cd ostis-metasystem
git remote add upstream git@github.com:ostis-ai/ostis-metasystem.git
```

* To update your `main` from `upstream` use:

```sh
git fetch upstream
git checkout upstream/main
```

* Use `git rebase` instead of `merge`. See [documentation about this command](https://git-scm.com/docs/git-rebase). To rebase your branch against main use:

```sh
git checkout <yourbranch>
git rebase upstream/main
```

* If you have any problems, then redo:

```sh
git rebase --abort
```

* Or ask in [Element](https://app.element.io/index.html#/room/#ostis_tech_support:matrix.org).

## Commits message format

Each commit message should be formed as: `[tag1]...[tagN] Message text (#issue)`.

Message text should start from an upper case letter. If commit doesn't fix or implement any #issue, then it shouldn't be pointed in commit message.

Examples:
<pre>
[solver] Implement TranslateMainSystemIdtfsFromScToFileAgent
[docs] Add docs for TranslateMainSystemIdtfsFromScToFileAgent
</pre>

Possible tags:

  * `[kb]` - changes in `sc-models/knowledge-base` folder;
  * `[solver]` - changes in `platform-dependent-components/problem-solver` and `sc-models/problem-solver` folder;
  * `[interface]` - changes in `platform-dependent-components/interface` and `sc-models/interface` folder;
  * `[tests]` or `[test]` - changes in tests;
  * `[config]` - commits with changes in configuration;
  * `[review]` - commits with review fixes;
  * `[refactor]` - commits with some code refactoring;
  * `[changelog]` - use when you update changelog;
  * `[docs]` or `[doc]` - use when you update documentation;
  * `[build]` - updates in build system;
  * `[cmake]` - updates in cmake files;
  * `[scripts]` - updates in the `ostis-metasystem/scripts` files;
  * `[ci]` - changes in `ci` configuration or scripts;
  * `[git]` - changes in `git` configuration.

Each commit in Pull Request should be an atomic. In other words, it should implement or fix one feature. For example:

<pre>
Last commit
...
[tests] Test TranslateMainSystemIdtfsFromScToFileAgent
[changelog] Add info about TranslateMainSystemIdtfsFromScToFileAgent
...
Init commit
</pre>

***
Each commit should have not much differences excluding cases, with:

  * CodeStyle changes; 
  * Renames; 
  * Code formatting.

**Do atomic commits for each changes.** For example if you rename some members in `ClassX` and `ClassY`, then do two commits:
<pre>
[refactor] Rename members in ClassX according to codestyle
[refactor] Rename members in ClassY according to codestyle
</pre>

**Do not mix codestyle changes and any logical fixes in one commit.**

All commits that not follow these rules should be split according to these rules. Otherwise they will be rejected with Pull Request.

***
## Pull Request

Each Pull Request with many changes, that not possible to review (excluding codestyle, rename changes), will be rejected.

### Pull Request Preparation

 - Read rules to create PR in documentation;
 - Update changelog;
 - Update documentation;
 - Cover new functionality with tests;
 - Your code should be written according to a codestyle like in sc-machine (see [Codestyle rules](https://ostis-ai.github.io/sc-machine/dev/codestyle/)).

### Pull Request creation

 - Create PR on GitHub;
 - Check that CI checks were passed successfully.

### Pull Request Review

 - Reviewer tests code from PR if CI doesn't do it;
 - Reviewer submits review as set of conversations;
 - Author makes review fixes at `Review fixes` commits;
 - Author re-requests review;
 - Reviewer resolves conversations and approves PR if conversations were fixed.
