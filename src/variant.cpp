/*
  Fairy-Stockfish, a UCI chess variant playing engine derived from Stockfish
  Copyright (C) 2018-2020 Fabian Fichter

  Fairy-Stockfish is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Fairy-Stockfish is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "parser.h"
#include "piece.h"
#include "variant.h"

using std::string;

VariantMap variants; // Global object

namespace {
    // Define variant rules
    Variant* fairy_variant_base() {
        Variant* v = new Variant();
        v->pieceToCharTable = "PNBRQ................Kpnbrq................k";
        v->endgameEval = false;
        return v;
    }
    Variant* chess_variant() {
        Variant* v = fairy_variant_base();
        v->endgameEval = true;
        return v;
    }
    Variant* chess960_variant() {
        Variant* v = chess_variant();
        v->chess960 = true;
        return v;
    }
    Variant* fairy_variant() {
        Variant* v = chess_variant();
        v->add_piece(SILVER, 's');
        v->add_piece(FERS, 'f');
        return v;
    }
    // Makruk (Thai Chess)
    Variant* makruk_variant() {
        Variant* v = chess_variant();
        v->variantTemplate = "makruk";
        v->pieceToCharTable = "PN.R.M....SKpn.r.m....sk";
        v->remove_piece(BISHOP);
        v->remove_piece(QUEEN);
        v->add_piece(KHON, 's');
        v->add_piece(MET, 'm');
        v->startFen = "rnsmksnr/8/pppppppp/8/8/PPPPPPPP/8/RNSKMSNR w - - 0 1";
        v->promotionRank = RANK_6;
        v->promotionPieceTypes = {MET};
        v->doubleStep = false;
        v->castling = false;
        v->nMoveRule = 0;
        v->countingRule = MAKRUK_COUNTING;
        return v;
    }
	Variant* legan_variant() {
	    Variant* v = chess_variant();
	    v->remove_piece(PAWN); 
	    v->add_piece(BREAKTHROUGH_PIECE, 'p');
	    v->promotionPieceTypes = {QUEEN, ROOK, BISHOP, KNIGHT};
	   v->startFen = "knbrp3/bqpp4/npp5/rp1p3P/p3P1PR/5PPN/4PPQB/3PRBNK w - - 0 1";
        v->doubleStep = false; 
	   v->firstRankDoubleSteps = false;
       return v;
    } 
    Variant* breakthrough_variant() {
        Variant* v = fairy_variant_base();
        v->pieceToCharTable = "P.................p.................";
        v->reset_pieces();
        v->add_piece(BREAKTHROUGH_PIECE, 'p');
        v->startFen = "pppppppp/pppppppp/8/8/8/8/PPPPPPPP/PPPPPPPP w 0 1";
        v->promotionPieceTypes = {};
        v->firstRankDoubleSteps = false;
        v->castling = false;
        v->stalemateValue = -VALUE_MATE;
        v->flagPiece = BREAKTHROUGH_PIECE;
        v->whiteFlag = Rank8BB;
        v->blackFlag = Rank1BB;
        return v;
    }
	Variant* breakthrough5_variant() {
        Variant* v = fairy_variant_base();
        v->pieceToCharTable = "P.................p.................";
        v->maxRank = RANK_5;
        v->maxFile = FILE_E;
		v->reset_pieces();
        v->add_piece(BREAKTHROUGH_PIECE, 'p');
        v->startFen = "ppppp/ppppp/5/PPPPP/PPPPP w 0 1";
        v->promotionPieceTypes = {};
        v->firstRankDoubleSteps = false;
        v->castling = false;
        v->stalemateValue = -VALUE_MATE;
        v->flagPiece = BREAKTHROUGH_PIECE;
        v->whiteFlag = Rank5BB;
        v->blackFlag = Rank1BB;
        return v;
    }
	Variant* breakthrough6_variant() {
        Variant* v = fairy_variant_base();
        v->pieceToCharTable = "P.................p.................";
        v->maxRank = RANK_6;
        v->maxFile = FILE_F;
		v->reset_pieces();
        v->add_piece(BREAKTHROUGH_PIECE, 'p');
        v->startFen = "pppppp/pppppp/6/6/PPPPPP/PPPPPP w 0 1";
        v->promotionPieceTypes = {};
        v->firstRankDoubleSteps = false;
        v->castling = false;
        v->stalemateValue = -VALUE_MATE;
        v->flagPiece = BREAKTHROUGH_PIECE;
        v->whiteFlag = Rank6BB;
        v->blackFlag = Rank1BB;
        return v;
    }
	
	Variant* breakthrough7_variant() {
        Variant* v = fairy_variant_base();
        v->pieceToCharTable = "P.................p.................";
        v->maxRank = RANK_7;
        v->maxFile = FILE_G;
		v->reset_pieces();
        v->add_piece(BREAKTHROUGH_PIECE, 'p');
        v->startFen = "ppppppp/ppppppp/7/7/7/PPPPPPP/PPPPPPP w 0 1";
        v->promotionPieceTypes = {};
        v->firstRankDoubleSteps = false;
        v->castling = false;
        v->stalemateValue = -VALUE_MATE;
        v->flagPiece = BREAKTHROUGH_PIECE;
        v->whiteFlag = Rank6BB;
        v->blackFlag = Rank1BB;
        return v;
    }
   
#ifdef LARGEBOARDS
    Variant* capablanca_variant() {
        Variant* v = chess_variant();
        v->pieceToCharTable = "PNBRQ..AC............Kpnbrq..ac............k";
        v->maxRank = RANK_8;
        v->maxFile = FILE_J;
        v->castlingKingsideFile = FILE_I;
        v->castlingQueensideFile = FILE_C;
        v->add_piece(ARCHBISHOP, 'a');
        v->add_piece(CHANCELLOR, 'c');
        v->startFen = "rnabqkbcnr/pppppppppp/10/10/10/10/PPPPPPPPPP/RNABQKBCNR w KQkq - 0 1";
        v->promotionPieceTypes = {ARCHBISHOP, CHANCELLOR, QUEEN, ROOK, BISHOP, KNIGHT};
        return v;
    }
   
#endif

} // namespace


/// VariantMap::init() is called at startup to initialize all predefined variants

void VariantMap::init() {
    // Add to UCI_Variant option
    add("chess", chess_variant());
    add("normal", chess_variant());
    //add("fischerandom", chess960_variant());
    //add("nocastle", nocastle_variant());
    add("fairy", fairy_variant()); // fairy variant used for endgame code initialization
    add("legan", legan_variant());
	/*add("makruk", makruk_variant());
    add("makpong", makpong_variant());
    add("cambodian", cambodian_variant());
    add("karouk", karouk_variant());
    add("asean", asean_variant());
    add("ai-wok", aiwok_variant());
    add("shatranj", shatranj_variant());
    add("amazon", amazon_variant());
    add("hoppelpoppel", hoppelpoppel_variant());
    add("newzealand", newzealand_variant());
    add("kingofthehill", kingofthehill_variant());
    add("racingkings", racingkings_variant());
    add("knightmate", knightmate_variant());
    add("losers", losers_variant());
    add("giveaway", giveaway_variant());
    add("antichess", antichess_variant());
    add("suicide", suicide_variant());
    add("codrus", codrus_variant());
    add("extinction", extinction_variant());
    add("kinglet", kinglet_variant());
    add("threekings", threekings_variant());
    add("horde", horde_variant());
    add("3check", threecheck_variant());
    add("5check", fivecheck_variant());
    add("crazyhouse", crazyhouse_variant());
    add("loop", loop_variant());
    add("chessgi", chessgi_variant());
    add("bughouse", bughouse_variant());
    add("koedem", koedem_variant());
    add("pocketknight", pocketknight_variant());
    add("placement", placement_variant());
    add("sittuyin", sittuyin_variant());
    add("seirawan", seirawan_variant());
    add("shouse", shouse_variant());
    add("minishogi", minishogi_variant());
    add("mini", minishogi_variant());
    add("kyotoshogi", kyotoshogi_variant());
    add("micro", microshogi_variant());
    add("dobutsu", dobutsu_variant());
    add("gorogoro", gorogoroshogi_variant());
    add("judkins", judkinsshogi_variant());
    add("euroshogi", euroshogi_variant());
    add("losalamos", losalamos_variant());
    add("gardner", gardner_variant());
    add("almost", almost_variant());
    add("chigorin", chigorin_variant());
    add("shatar", shatar_variant());
    add("clobber", clobber_variant());
    add("breakthrough", breakthrough_variant());
	add("breakthrough5", breakthrough5_variant());
	add("breakthrough6", breakthrough6_variant());
	add("breakthrough7", breakthrough7_variant());
    add("minixiangqi", minixiangqi_variant()); */
#ifdef LARGEBOARDS
    /*add("shogi", shogi_variant());
    add("capablanca", capablanca_variant());
    add("capahouse", capahouse_variant());
    add("caparandom", caparandom_variant());
    add("gothic", gothic_variant());
    add("janus", janus_variant());
    add("modern", modern_variant());
    add("chancellor", chancellor_variant());
    add("embassy", embassy_variant());
    add("centaur", centaur_variant());
    add("jesonmor", jesonmor_variant());
    add("courier", courier_variant());
    add("grand", grand_variant());
    add("shako", shako_variant());
    add("clobber10", clobber10_variant());
    add("xiangqi", xiangqi_variant());
    add("manchu", manchu_variant());
    add("supply", supply_variant());
    add("janggi", janggi_variant());
    add("janggitraditional", janggi_traditional_variant());
    add("janggicasual", janggi_casual_variant());*/
#endif
}


/// VariantMap::parse reads variants from an INI-style configuration file.

template <bool DoCheck>
void VariantMap::parse(std::string path) {
    if (path.empty() || path == "<empty>")
        return;
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "Unable to open file " << path << std::endl;
        return;
    }
    std::string variant, variant_template, key, value, input;
    while (file.peek() != '[' && std::getline(file, input)) {}

    std::vector<std::string> varsToErase = {};
    while (file.get() && std::getline(std::getline(file, variant, ']'), input))
    {
        // Extract variant template, if specified
        if (!std::getline(std::getline(std::stringstream(variant), variant, ':'), variant_template))
            variant_template = "";

        // Read variant rules
        Config attribs = {};
        while (file.peek() != '[' && std::getline(file, input))
        {
            std::stringstream ss(input);
            if (ss.peek() != '#' && std::getline(std::getline(ss, key, '=') >> std::ws, value) && !key.empty())
                attribs[key.erase(key.find_last_not_of(" ") + 1)] = value;
        }

        // Create variant
        if (variants.find(variant) != variants.end())
            std::cerr << "Variant '" << variant << "' already exists." << std::endl;
        else if (!variant_template.empty() && variants.find(variant_template) == variants.end())
            std::cerr << "Variant template '" << variant_template << "' does not exist." << std::endl;
        else
        {
            if (DoCheck)
                std::cerr << "Parsing variant: " << variant << std::endl;
            Variant* v = !variant_template.empty() ? VariantParser<DoCheck>(attribs).parse(new Variant(*variants.find(variant_template)->second))
                                                   : VariantParser<DoCheck>(attribs).parse();
            if (v->maxFile <= FILE_MAX && v->maxRank <= RANK_MAX)
            {
                add(variant, v);
                // In order to allow inheritance, we need to temporarily add configured variants
                // even when only checking them, but we remove them later after parsing is finished.
                if (DoCheck)
                    varsToErase.push_back(variant);
            }
            else
                delete v;
        }
    }
    file.close();
    // Clean up temporary variants
    for (std::string tempVar : varsToErase)
    {
        delete variants[tempVar];
        variants.erase(tempVar);
    }
}

template void VariantMap::parse<true>(std::string path);
template void VariantMap::parse<false>(std::string path);

void VariantMap::add(std::string s, const Variant* v) {
  insert(std::pair<std::string, const Variant*>(s, v));
}

void VariantMap::clear_all() {
  for (auto const& element : *this)
      delete element.second;
  clear();
}

std::vector<std::string> VariantMap::get_keys() {
  std::vector<std::string> keys;
  for (auto const& element : *this)
      keys.push_back(element.first);
  return keys;
}
