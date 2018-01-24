This Report Template can be used offline locally (if you have installed a LaTeX document Processing system) or online at overleaf.com.
The LaTeX Report Template re-uses an IEEE Transactions Article Class and Template With some customizations.

!!! Do not modify the ‘header’ of the files as they contain compile directives.!

To start using the IST-PORTFOLIO-Report LaTeX template, just open and modify your data in the following files:

1. PP-Report-Preamble.tex
	On line 16 select the ‘main’ language of your Report in the line that loads
	the package ‘babel’ (just change the main= to the adequate language):

	\usepackage[main=english,portuguese]{babel}
	
	or 
    
	\usepackage[english,main=portuguese]{babel}
	
	The Report document will then "auto-magically" modify all the adequate key
	formatting components, such as some headings, captions naming and numbering
	styles, etc. according to the selected language.


2. PP—Report-Cover.tex
	In this document insert your data (authors, student IDs, titles, date, etc.).
	Start on line 13 with the Title of your document.
	On line 27, if only one author, then leave just one name in command \author.
	On lines 34 to 39 put again the name(s), ID(s) and email address(es).
	On line 41 insert the date of submission
	On line 45 put an abridged Title of the Report.
	On line 47 or line 49 select (by uncommenting) the line of interest and fill
	the data for either a Group Report or for an Individual Report.

3. PP-Report.tex
	On line 15 select the type of Report (Activity or Learning).
	For that purpose change the argument of command "ReportType" to
    the value "act" for Activity Report, or "learn" for Learnings Report. 

	The CONTENT of your Report will be written in this file.
	Start with the Abstract text as illustrated in the template.
	After compilation it produces a .pdf document.
	In overleaf.com, you can download everything as .zip file and also the PDF
	document that is produced.
    In overleaf.com it is also possible to keep "versions" of the docuement while
    You write it (see top menu "History and Revisions"
	If used with a local LaTeX distribution, such as TeXLive/MacTeX just use
	the PDF document produced.  

4. PP_Report_bib.bib or Mendeley.bib
	This file is the Bibliography database. Just replace it with your own,
	and if the name is different, just modify the corresponding line in in the
	main document ‘PP—Report.tex’ (near the end of the file) such as:

	\bibliography{./PP_Report_bib}
    
	If you use as Reference Manager for your bibliographies the excellent
	Mendeley.com (and the desktop/tablet app), you can link your bibliography
	to your report in order to have it automatically updated.
	For that option in overleaf.com you just need to go to "Add files..." menu
	(top left of your Project pane in overleaf.com, and select "Bibliography".
	A pop-up window will then show up a few options, where you can find Mendeley.
 	Select it, that authorize with you credentials (of Mendeley).
	The system will then ask you the name of the bibliography file to synchronize
	(Mendeley.bib by default).
	In the PP-Report.tex just have the following line modified accordingly:

	\bibliography{./Mendeley}

#####################

Enjoy

Rui Santos Cruz