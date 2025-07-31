
# Contributing to CppUnit to Gtest

Thank you for your interest in contributing to this project! We welcome contributions of all kinds, from bug reports to feature implementations.

## Getting Started

### Prerequisites

- CMake 3.10 or later

### Setting Up Your Development Environment

1. **Fork the repository** on GitHub
2. Clone your fork locally
3. Build the project and run tests:
   ```bash
   cmake -B build -S . -Dbuild_testing=ON -DBuildExamples=ON -DBuildInternalTests=ON
   cmake --build build
   ctest --test-dir build
   ```

## How to Contribute

### Reporting Issues

Before creating a new issue:
- Search existing issues to avoid duplicates
- Check if the issue exists in the latest version

When reporting bugs, please include:
- A clear, descriptive title
- Steps to reproduce the issue
- Expected vs actual behavior
- Minimal code example demonstrating the problem
- Your environment (OS, compiler, versions)

For feature requests:
- Describe the CppUnit feature you need supported
- Provide an example test case showing the desired functionality

### Making Changes

1. **Create an issue first**
   - Discuss your proposed changes before implementing
   - This helps avoid duplicate work and ensures alignment with project goals

2. **Create a feature branch** from your fork's main branch
   - Branch names should include the issue number: `123-add-test-timeout-support` or `456-fix-assertion-messages`
   - Use descriptive names that explain what the branch does

3. **Make your changes**
   - Follow the existing code style and conventions
   - Add tests for new functionality
   - Update documentation as needed
   - Ensure your changes don't break existing functionality

4. **Test your changes thoroughly**
   - Run the full test suite: `ctest --test-dir build`
   - Test with both the examples and internal tests
   - Add real CppUnit test cases to examples if required

5. **Submit a Pull Request**
   - Reference the issue number in your PR description (i.e. "Closes #42") 
   - Provide a clear description of what your changes do
   - Explain why the changes are necessary
   - Include any breaking changes or migration notes

## Code Style Guidelines

- Follow the existing code formatting and style
- Use clear, descriptive variable and function names, adding a trailing `_` for possible conflicts.
- Add comments for complex logic or non-obvious implementations
- Keep functions focused and reasonably sized
- All code must compile under C++14 (preferably 11)

## Testing Requirements

All contributions must include appropriate tests:

- **New features**: Add tests demonstrating the feature works correctly
- **Bug fixes**: Add regression tests to prevent the bug from recurring
- **Changes to existing code**: Ensure all existing tests continue to pass

Test categories:
- **Examples**: Tests based on CppUnit's official examples
- **Internal tests**: Tests for the adapter's internal functionality
- **Edge cases**: Tests for unusual or boundary conditions

## Review Process

1. **Automated checks**: All PRs must pass CI checks
   - All tests must pass
   - Code coverage must remain at 100%
   - Surving mutants with [Mull](https://github.com/mull-project/mull) must remain low.

2. **Manual review**: A maintainer will review your PR
   - Be patient - reviews may take time
   - Address feedback promptly and professionally
   - Be open to suggestions and alternative approaches

3. **Approval and merge**: Once approved, a maintainer will merge your PR

## Project Scope

This project focuses on:
- Supporting CppUnit features, including deprecated ones
- Maintaining compatibility with existing CppUnit test code
- Providing a smooth migration path to Google Test

We generally do **not** aim to:
- Support every obscure CppUnit feature that lacks clear documentation or examples
- Reimplement CppUnit's entire API beyond what's needed for test compatibility

If you're unsure whether a feature fits the project scope, please create an issue to discuss it first.

## Questions or Need Help?

- Create an issue for questions about contributing
- Tag maintainers if you need clarification
- Be patient and polite in all interactions

## Code of Conduct

- Be respectful and professional in all interactions
- Focus on constructive feedback and solutions
- Help create a welcoming environment for all contributors
- Report any inappropriate behavior to the maintainers

Thank you for contributing to make CppUnit to Gtest better for everyone!
