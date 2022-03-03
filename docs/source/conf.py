#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import subprocess

on_rtd = os.environ.get('READTHEDOCS', None) == 'True'

if on_rtd:
    subprocess.call('cd ..; doxygen Doxyfile.in', shell=True)

import sphinx_rtd_theme

html_theme = "sphinx_rtd_theme"

html_theme_path = [sphinx_rtd_theme.get_html_theme_path()]

#def setup(app):
#    app.add_css_file("main_stylesheet.css")

extensions = ['breathe','sphinx.ext.imgmath',]
breathe_projects = { 'tmech': '_build/xml' }
breathe_default_project = "tmech"
templates_path = ['_templates']
html_static_path = ['_static']
source_suffix = '.rst'
master_doc = 'index'
project = 'tmech'
copyright = '2022, Peter Lenz'
author = 'Peter Lenz'

#html_logo = 'logo'
imgmath_image_format = 'svg'

imgmath_latex_preamble = r'''
\usepackage{amsmath}
\usepackage{amsfonts}
\usepackage{amssymb}
\usepackage{mathrsfs}
 \newcommand{\inv}{^{-1}}
\newcommand{\invT}{^{-T}}
\newcommand{\eye}{\mathbf{I}}
\newcommand{\Eye}{\mathbb{I}}

\newcommand{\Basis}[1]{\mathbf{e}_{#1}}
\newcommand{\Ei}{\Basis{i}}
\newcommand{\Ej}{\Basis{j}}
\newcommand{\Ek}{\Basis{k}}
\newcommand{\Em}{\Basis{m}}
\newcommand{\En}{\Basis{n}}
\newcommand{\Eo}{\Basis{o}}
\newcommand{\Ep}{\Basis{p}}
\newcommand{\BasisSecond}[2]{\mathbf{e}_{#1}\otimes\mathbf{e}_{#2}}
\newcommand{\BasisFourth}[4]{\BasisSecond{#1}{#2}\otimes\BasisSecond{#3}{#4}}
\newcommand{\BasisSixth}[6]{\BasisSecond{#1}{#2}\otimes\BasisSecond{#3}{#4}\otimes\BasisSecond{#5}{#6}}
\newcommand{\BasisEigth}[8]{\BasisFourth{#1}{#2}{#3}{#4}\otimes\BasisFourth{#5}{#6}{#7}{#8}}
\newcommand{\FirstT}[1]{\boldsymbol{#1}}
\newcommand{\SecondT}[1]{\mathbf{#1}}
\newcommand{\FourthT}[1]{\mathbb{#1}}
\newcommand{\SixthT}[1]{\overset{6}{\mathbb{#1}}}
\newcommand{\EightT}[1]{\overset{8}{\mathbb{#1}}}

\newcommand{\contractio}{
\setlength{\unitlength}{1cm}
\begin{picture}(0.3,0.2)
\put(0.08,0.05){\circle*{0.08}}\put(0.23,0.05){\circle{0.08}}
\end{picture}
}

\newcommand{\contractoi}{
\setlength{\unitlength}{1cm}
\begin{picture}(0.3,0.2)
\put(0.08,0.05){\circle{0.08}}\put(0.23,0.05){\circle*{0.08}}
\end{picture}}


\newcommand{\dcontractoi}{
\setlength{\unitlength}{1cm}
\begin{picture}(0.3,0.2)
\put(0.08,0.15){\circle{0.08}}\put(0.23,0.15){\circle*{0.08}}
\put(0.08,0.05){\circle{0.08}}\put(0.23,0.05){\circle*{0.08}}
\end{picture}}

\newcommand{\dcontractio}{
\setlength{\unitlength}{1cm}
\begin{picture}(0.3,0.2)
\put(0.08,0.15){\circle*{0.08}}\put(0.23,0.15){\circle{0.08}}
\put(0.08,0.05){\circle*{0.08}}\put(0.23,0.05){\circle{0.08}}
\end{picture}}

\newcommand{\StrainT}{\boldsymbol{\varepsilon}}
\newcommand{\StressT}{\boldsymbol{\sigma}}
 '''


exclude_patterns = []
highlight_language = 'c++'
pygments_style = 'sphinx'
todo_include_todos = False
htmlhelp_basename = 'tmechdoc'

html_js_files = [
    'goatcounter.js'
]

# Automatically link to numpy doc
extensions += ['sphinx.ext.intersphinx']
intersphinx_mapping = {
    "numpy": ("https://numpy.org/doc/stable/", None),
    "scipy": ("https://docs.scipy.org/doc/scipy/reference", None),
}


# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# import os
# import sys
# sys.path.insert(0, os.path.abspath('.'))
#from sphinx.builders.html import StandaloneHTMLBuilder
#import subprocess, os

# Doxygen
#subprocess.call('doxygen Doxyfile.in', shell=True)



#import os
#import subprocess

#on_rtd = os.environ.get('READTHEDOCS', None) == 'True'

#if on_rtd:
#subprocess.call('doxygen Doxyfile.in', shell=True)

#import sphinx_rtd_theme

#html_theme = "sphinx_rtd_theme"

#html_theme_path = [sphinx_rtd_theme.get_html_theme_path()]





# -- Project information -----------------------------------------------------

#project = 'tmech'
#copyright = '2021, Peter Lenz'
#author = 'Peter Lenz'


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
#extensions = [
#    'sphinx.ext.autodoc',
#    'sphinx.ext.intersphinx',
#    'sphinx.ext.autosectionlabel',
#    'sphinx.ext.todo',
#    'sphinx.ext.coverage',
#    #'sphinx.ext.mathjax',
#    'sphinx.ext.imgmath',
#    'sphinx.ext.ifconfig',
#    'sphinx.ext.viewcode',
#    'sphinx.ext.inheritance_diagram',
#    'breathe',
#    #'sphinxcontrib.bibtex',
#]

#bibtex_bibfiles = ['api/refs.bib']


#imgmath_image_format = 'svg'



#mathjax_config = {
#    'extensions': ['tex2jax.js'],
#    'jax': ['input/tex-full', 'output/HTML-CSS'],
#}

# Add any paths that contain templates here, relative to this directory.
#templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
#exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

#highlight_language = 'c++'

# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
#html_theme = 'sphinx_rtd_theme'
#html_theme_options = {
#    'canonical_url': '',
#    'analytics_id': '',  #  Provided by Google in your dashboard
#    'display_version': True,
#    'prev_next_buttons_location': 'bottom',
#    'style_external_links': False,
#    
#    'logo_only': False,#

    # Toc options
#    'collapse_navigation': True,
#    'sticky_navigation': True,
#    'navigation_depth': 4,
#    'includehidden': True,
#    'titles_only': False
#}
# html_logo = ''
# github_url = ''
# html_baseurl = ''

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
#html_static_path = ['_static']

#master_doc = 'index'


# -- Breathe configuration -------------------------------------------------

#breathe_projects = {
#	"tmech": "_build/xml/"
#}
#breathe_default_project = "tmech"
#breathe_default_members = ('members', 'undoc-members')
#breathe_default_function_domain = "cpp"
#breathe_domain_by_extension = {"c": "c", "h": "c"}
