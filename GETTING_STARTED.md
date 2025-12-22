# Getting Started

## What is an Environment Query?

## Key Concepts

**Context** - This is a reference point to any Generator or Test, which can be a single Node, or entire groups of nodes. For example, Generators will generate points around each node in the context. Contexts can be Nodes or just positions.

**Generator** - They produce potential locations (called Items) that will then be evaluated by the tests for testing.

**Test** - These are used to decide which of the generated Items is the best, multiple tests can be used for more control over the final result. For example, one can test whether a Context is in line of sight of each Item, or how far that Context is.

## Installation

## GEQO Nodes Overview

## Setting Up Contexts

## Crafting a Query

- The step-by-step:
  1. Choose Context(s)
  2. Add EnvironmentQuery
  3. Add Generator
  4. Add Test
  5. Run the query
  6. Read result
  7. How to debug

## Example Usage

## Usage With Godot's Pathfinding
