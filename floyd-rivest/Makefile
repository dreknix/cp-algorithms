CC=clang
CFLAGS=-O3 -Wall

CXX=clang++
CXXFLAGS=-O3 -Wall

.PHONY: all lint lint-cpp lint-tidy lint-tidy-detail format zip

all: k_lowest

k_lowest: k_lowest.o
	${CC} ${CFLAGS} -o $@ $^ -lm

%.o: %.c
	${CC} ${CFLAGS} -o $@ -c $<

%.o: %.cc
	${CXX} ${CXXFLAGS} -o $@ -c $<

clean:
	@rm -rf *.o sort-quicksort

lint: lint-cpp lint-tidy lint-shell lint-md

lint-cpp:
	@echo -n "\nC coding style (CPPLINT): "
	@if cpplint --quiet --recursive . ; then \
		echo "accepted" ; \
	else \
		echo "violated" ; \
		exit 1 ; \
	fi

## clang-tidy config
# add these checks to .clang-tidy
#CLANG-TIDY-CHECKS=-checks=bugprone-*,cert-*,cppcoreguidelines-*,mordernize-*,portability-*,readability-*,clang-analyzer-*,misc-*,-cppcoreguidelines-init-variables,-cert-msc3*,-cert-msc5*,-cppcoreguidelines-avoid-magic-numbers,-readability-magic-numbers
# disable readability-identifier-naming until next semester
CLANG-TIDY-CHECKS=-checks=-readability-identifier-naming
lint-tidy:
	@echo "\nC coding style (clang-tidy): "
	@clang-tidy **/*.c ${CLANG-TIDY-CHECKS} -- -std=c99 -Wall 2>&1 | grep "^/" | cut -f 1 -d ":" | uniq -c

lint-tidy-detail:
	@clang-tidy **/*.c ${CLANG-TIDY-CHECKS} -- -std=c99 -Wall || true

format:
	@echo "\nFormat source code: "
	@for f in $$(find . -type f -name "*.[hc]" | sort) ; do \
		cat $$f | clang-format --style=file | diff -q $$f - ; \
	done || true

format-detail:
	@for f in $$(find . -type f -name "*.[hc]" | sort) ; do \
		cat $$f | clang-format --style=file | diff -u - $$f | delta --line-numbers --side-by-side --syntax-theme base16-256 --diff-so-fancy --zero-style "dim syntax" --plus-style="syntax auto" ; \
	done || true
