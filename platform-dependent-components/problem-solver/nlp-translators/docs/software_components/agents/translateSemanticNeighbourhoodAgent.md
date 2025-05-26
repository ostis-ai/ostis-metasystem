This is an agent that generates a link that contains semantic neighbourhood of first argument of this agent's action.

**Action class:**

`action_form_semantic_neighbourhood_translation`

**Parameters:**

1. `node` - node which semantic neighbourhood should be converted to text.

**Workflow:**

* The agent iterates over translators that convert specific sc-construction into text;
* Each translator generates no more than `TranslationConstants::MAX_TRANSLATIONS_FROM_EACH_TRANSLATOR` translations;
* Translators skip nodes that are in `translation_ignored_keynodes` set;
* After results of all translators are collected agent shuffles them and concatenates `TranslationConstants::MAX_TRANSLATIONS_FROM_ALL_TRANSLATORS` of them;
* Agent creates link, sets it content to concatenated translations and adds it to answer structure.


**Comment:**

* You can create your own translator. To do that you should inherit `naturalLanguageProcessingModule::SemanticNeighbourhoodTranslator` and add your translator into set with all translators in `naturalLanguageProcessingModule::SemanticNeighbourhoodTranslatorSet`
* Basic translators only handle those nodes that have main identifier in English.

### Example

Example of an input structure:

<img src="../images/formSemanticNeighbourhoodTranslationAgentInput.png"></img>

Example of an output structure:

<img src="../images/formSemanticNeighbourhoodTranslationAgentOutput.png"></img>

### Agent implementation language
C++

### Result

Possible result codes:

* `sc_result_ok` - the agent work finished successfully;
* `sc_result_error`- agent parameters are not valid or there is an internal error.
