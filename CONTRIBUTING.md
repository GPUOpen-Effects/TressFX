# Contributing to GPUOpen Effects

Thank you for your interest in GPUOpen Effects.

Contributions can be made by creating a GitHub pull request.

---

**IMPORTANT**: By creating a pull request, you agree to allow your contribution to be licensed by the project owners under the terms of the [MIT License](LICENSE.txt).

---

### Types of contributions
We welcome pull requests without prior discussion for the following types of contributions:
* Typo fixes
* Bug fixes (that do not require significant code changes)
* Shader performance improvements (that do not require significant code changes)

We also welcome pull requests for new features and improvements/optimizations/bug fixes that require larger code changes. However, these should be discussed prior to starting work, via GitHub issues. This is to avoid a contributor spending a lot of time on changes that might not be accepted. In general, the bigger the change, the more discussion should take place before starting work.

Regarding shader performance improvements, it is possible to make a change that runs faster on one GPU architecture but slower on another. Shader changes should not reduce performance or quality on any supported GPU. We recognize that some contributors may not have different GPUs for testing. GPUOpen maintainers will evaluate shader changes to ensure performance and quality are not degraded on different GPU architectures.

### Implementation guidelines
When contributing to an existing GPUOpen Effects library or sample, we mostly just want your code to be:

1. Useful to game developers and other users of real-time visual effects code
2. Mindful of performance
3. Written in the style of the existing code

Here are a few specific guidelines:

* **Match the style of existing code**
  * This is the most important guideline
* Avoid C++11/14/17 features
  * Possible exceptions include simple changes/additions to the language that are widely supported
  * "Widely supported" means it works all the way back to Visual Studio 2010
  * We allow `nullptr`, for example, because it is:
    1. A minor change compared to "ordinary" C++ (i.e. C++98/03)
    2. An improvement over "ordinary" C++
    3. Widely supported
* Avoid adding new dependencies
* Follow the rules for namespaces
  * Wrap external API functions in the AMD namespace
  * Avoid namespace `using` declarations in header files and before `#include` directives in implementation files
  * Avoid unnamed namespaces in header files
* Avoid complicated template code
  * We realized that "complicated" is a subjective standard
  * If you decide to write template code to avoid manually writing several specialized versions of the same function, and there is an actual need for different versions of the function, and the resulting code is not a tangle of angle brackets, then your template code may qualify as "uncomplicated"
  * But be aware that many of the maintainers view template syntax as inherently ugly, and while recognizing there are good uses for templates, believe that their overuse leads to code that is less readable and harder to maintain
* Avoid vendor-specific shader code, for simplicity and readability
  * Use only vanilla HLSL, at least for now
  * Avoid conditional compilation based on vendor, at least for now
  * We may relax or eliminate this rule in the future

### Code formatting
The code formatting guidelines help keep the look of the code (at least somewhat) consistent within a particular GPUOpen Effects project as well as across all GPUOpen Effects projects.

We recognize that some of the guidelines below are completely in the realm of personal preference (e.g. spaces vs. tabs, formatting of `if` statements and `for` loops, placement of curly braces, etc.). But consistency is important, and in order to achieve consistency, a style must be chosen.

For formatting considerations not covered by the guidelines below, **match existing code**.

Here are the guidelines for code formatting:

* Leave line endings as they are
  * Currently, all GPUOpen Effects are based solely on DirectX&reg; and thus Windows&reg;
  * All text files use CRLF line endings
  * Our .gitattributes files enforce this, so you should not have to worry about it
* Avoid trailing whitespace
  * This includes blank lines, meaning blank lines should actually be empty (except for the line ending)
* Use spaces, not tabs
* Limit line length to a maximum of 200 characters
  * Widescreen, high-res monitors eliminate the need for an 80-character limit
  * But we still need lines to be limited to some reasonable length
  * Most lines should be limited to 100 characters, for fans of side-by-side window layouts, side-by-side diffs, etc.
* Use one space between conditional and loop keywords and the opening parenthesis
  * `if (result != AOFX_RETURN_CODE_SUCCESS)`
  * `for (int i = 0; i < m_MultiResLayerCount; i++)`
* Place curly braces from the same pair in the same line or in the same column
  * This:
    ```
    if (context == NULL) { return; }
    
    Mesh::~Mesh()
    {
        Release();
    }
    ```
  * Not this:
    ```
    if (context == NULL) {
        return; }
    
    Mesh::~Mesh() {
        Release();
    }
    ```
  * The one exception to "no K&R-style braces" is `extern "C" {`
* Ensure control structures have a complete curly-braced block of code
  * This:
    ```
    if (context == NULL) { return; }
    
    if (disabled)
    {
        return;
    }
    ```
  * Not this:
    ```
    if (context == NULL) return;
    
    if (disabled)
        return;
    ```

### Commit messages
Please follow the standard Git conventions for commit messages.

Here is our summary of these conventions, adapted somewhat for GitHub's particular rules:

* Write a subject line followed by a blank line followed by further explanation (if needed)
* Capitalize the subject line
* Do not put a period at the end of the subject line
* Limit the subject line to 72 characters
  * A common recommendation is to limit it to 50 characters
  * GitHub truncates subject lines longer than 72 characters
* Use the imperative present tense
  * e.g. "Fix rendering bug"
  * Not "Fixed rendering bug", "Fixes rendering bug", or "Fixing rendering bug"
* Wrap the body (i.e. the "further explanation" part) at 72 characters

If you are using GitHub Desktop, the blank line and text wrapping will be handled for you.

More information on the standard conventions for good commit messages can be found here:

* GitHub blog entry on [Shiny new commit styles](https://github.com/blog/926-shiny-new-commit-styles)
* Chris Beams' post on [How to Write a Git Commit Message](http://chris.beams.io/posts/git-commit/)
* Tim Pope's original [Note About Git Commit Messages](http://tbaggery.com/2008/04/19/a-note-about-git-commit-messages.html)

In addition, if the commit fixes an open issue, add `Fixes issue_number` to the end of the commit message:

* e.g. `Fixes #1`
* See [Closing issues via commit messages](https://help.github.com/articles/closing-issues-via-commit-messages/) for more details
