add_custom_target(
    proposal_pdf
    ALL
    COMMAND pdflatex -interaction=nonstopmode ${CMAKE_CURRENT_SOURCE_DIR}/proposal.tex || (exit 0)
    COMMAND bibtex proposal || (exit 0)
    COMMAND pdflatex -interaction=nonstopmode ${CMAKE_CURRENT_SOURCE_DIR}/proposal.tex || (exit 0)
    COMMAND pdflatex -interaction=nonstopmode ${CMAKE_CURRENT_SOURCE_DIR}/proposal.tex || (exit 0)
    COMMAND cp proposal.pdf ${CMAKE_SOURCE_DIR}/pdf
)

add_custom_target(
    proposal_presentation_pdf
    ALL
    COMMAND pdflatex -interaction=nonstopmode ${CMAKE_CURRENT_SOURCE_DIR}/proposal_presentation.tex || (exit 0)
    COMMAND bibtex proposal_presentation || (exit 0)
    COMMAND pdflatex -interaction=nonstopmode ${CMAKE_CURRENT_SOURCE_DIR}/proposal_presentation.tex || (exit 0)
    COMMAND pdflatex -interaction=nonstopmode ${CMAKE_CURRENT_SOURCE_DIR}/proposal_presentation.tex || (exit 0)
    COMMAND cp proposal_presentation.pdf ${CMAKE_SOURCE_DIR}/pdf
)

file(GLOB_RECURSE texfile RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "*.tex" "*.sty" "*.bib")
foreach( sourcefile IN LISTS texfile )
    configure_file(${sourcefile} ${sourcefile})
endforeach()

add_custom_target(
    diploma_pdf
    ALL
    COMMAND pdflatex -interaction=nonstopmode diploma.tex || (exit 0)
    COMMAND bibtex diploma || (exit 0)
    COMMAND pdflatex -interaction=nonstopmode diploma.tex || (exit 0)
    COMMAND pdflatex -interaction=nonstopmode diploma.tex || (exit 0)
    COMMAND cp diploma.pdf ${CMAKE_SOURCE_DIR}/pdf
)
