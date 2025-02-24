#pragma once

#include <vector>
#include "ofPoint.h"
#include "ofRectangle.h"
#include "ofConstants.h"
#include "ofPath.h"
#include "ofTexture.h"
#include "ofMesh.h"

/// \file
/// The ofTrueTypeFont class provides an interface to load fonts into
/// openFrameworks. The fonts are converted to textures, and can be drawn on
/// screen. There are some options when you load the font - what size the
/// font is rendered at, whether or not it is anti-aliased, and whether the
/// font object will be the full character set or a subset (i.e., extended
/// ASCII, which can include diacritics like umlauts, or ASCII). The default
/// is anti-aliased, non-full character set. The library uses freetype, which
/// has certain patent problems in regards to true type hinting, especially
/// at small sizes, so non-anti-aliased type doesn't always render
/// beautifully. But we find it quite adequate, and at larger sizes it seems
/// to works well.


/// \cond INTERNAL


typedef ofPath ofTTFCharacter;
typedef struct FT_FaceRec_*  FT_Face;

/// \endcond

/// \name Fonts
/// \{
static const std::string OF_TTF_SANS = "sans-serif";
static const std::string OF_TTF_SERIF = "serif";
static const std::string OF_TTF_MONO = "monospace";
/// \}


void ofTrueTypeShutdown();

class ofUnicode{
public:
	struct range{
		std::uint32_t begin;
		std::uint32_t end;
		
		std::uint32_t getNumGlyphs() const{
			return end - begin + 1;
		}
	};

	static const range Space;
	static const range Latin;
	static const range Latin1Supplement;
	static const range Greek;
	static const range Cyrillic;
	static const range Arabic;
	static const range ArabicSupplement;
	static const range ArabicExtendedA;
	static const range Devanagari;
	static const range HangulJamo;
	static const range VedicExtensions;
	static const range LatinExtendedAdditional;
	static const range GreekExtended;
	static const range GeneralPunctuation;
	static const range SuperAndSubScripts;
	static const range CurrencySymbols;
	static const range LetterLikeSymbols;
	static const range NumberForms;
	static const range Arrows;
	static const range MathOperators;
	static const range MiscTechnical;
	static const range BoxDrawing;
	static const range BlockElement;
	static const range GeometricShapes;
	static const range MiscSymbols;
	static const range Dingbats;
	static const range Hiragana;
	static const range Katakana;
	static const range HangulCompatJamo;
	static const range KatakanaPhoneticExtensions;
	static const range CJKLettersAndMonths;
	static const range CJKUnified;
	static const range DevanagariExtended;
	static const range HangulExtendedA;
	static const range HangulSyllables;
	static const range HangulExtendedB;
	static const range AlphabeticPresentationForms;
	static const range ArabicPresFormsA;
	static const range ArabicPresFormsB;
	static const range KatakanaHalfAndFullwidthForms;
	static const range KanaSupplement;
	static const range RumiNumericalSymbols;
	static const range ArabicMath;
	static const range MiscSymbolsAndPictographs;
	static const range Emoticons;
	static const range TransportAndMap;

};

class ofAlphabet{
public:
	static const std::initializer_list<ofUnicode::range> Emoji;
	static const std::initializer_list<ofUnicode::range> Japanese;
	static const std::initializer_list<ofUnicode::range> Chinese;
	static const std::initializer_list<ofUnicode::range> Korean;
	static const std::initializer_list<ofUnicode::range> Arabic;
	static const std::initializer_list<ofUnicode::range> Devanagari;
	static const std::initializer_list<ofUnicode::range> Latin;
	static const std::initializer_list<ofUnicode::range> Greek;
	static const std::initializer_list<ofUnicode::range> Cyrillic;
};




class ofTtfSettings{
	friend class ofTrueTypeFont;
	vector<ofUnicode::range> ranges;

public:
	ofTtfSettings(const string & name, int size)
	:fontName(name)
	,fontSize(size){}

	string fontName;
	int fontSize;
	bool antialiased = true;
	bool contours = false;
	float simplifyAmt = 0.3f;
	int dpi = 0;

	enum Direction{
		LeftToRight,
		RightToLeft
	};
	Direction direction = LeftToRight;

	void addRanges(std::initializer_list<ofUnicode::range> alphabet){
		ranges.insert(ranges.end(), alphabet);
	}

	void addRange(const ofUnicode::range & range){
		ranges.push_back(range);
	}
};


class ofTrueTypeFont{

public:


	/// \brief Construct a default ofTrueTypeFont.
	ofTrueTypeFont();

	/// \brief Destroy the ofTrueTypeFont.
	virtual ~ofTrueTypeFont();

	ofTrueTypeFont(const ofTrueTypeFont& mom);
	ofTrueTypeFont & operator=(const ofTrueTypeFont& mom);

	ofTrueTypeFont(ofTrueTypeFont&& mom);
	ofTrueTypeFont & operator=(ofTrueTypeFont&& mom);

	/// \name Load Font
	/// \{
				
	/// \brief Loads the font specified by filename, allows you to control size, aliasing, and other parameters.
	///
	/// loads a font, and allows you to set the following parameters: the filename, the size, if the font is anti-aliased,
	/// if it has a full character set, if you need it to have contours (for getStringPoints) and parameters that control 
	/// the simplification amount for those contours and the dpi of the font.
	/// 
	/// default (without dpi), non-full char set, anti aliased, 96 dpi
    ///
	/// \param filename The name of the font file to load.
    /// \param fontsize The size in pixels to load the font.
    /// \param _bAntiAliased true if the font should be anti-aliased.
    /// \param _bFullCharacterSet true if the full character set should be cached.
    /// \param makeControus true if the vector contours should be cached.
    /// \param simplifyAmt the amount to simplify the vector contours.  Larger number means more simplified.
    /// \param dpi the dots per inch used to specify rendering size.
	/// \returns true if the font was loaded correctly.
	bool load(string filename,
                  int fontsize,
                  bool _bAntiAliased=true,
                  bool _bFullCharacterSet=true,
                  bool makeContours=false,
                  float simplifyAmt=0.3f,
				  int dpi=0);

	OF_DEPRECATED_MSG("Use load instead",bool loadFont(string filename,
                  int fontsize,
                  bool _bAntiAliased=true,
                  bool _bFullCharacterSet=false,
                  bool makeContours=false,
                  float simplifyAmt=0.3f,
				  int dpi=0));
	
	bool load(const ofTtfSettings & settings);

	/// \brief Has the font been loaded successfully?
	/// \returns true if the font was loaded.
	bool isLoaded() const;

	/// \}
	/// \name Font Settings
	/// \{
	
	/// \brief Set the default dpi for all typefaces.
	static void setGlobalDpi(int newDpi);
	
	/// \brief Is the font anti-aliased?
	/// \returns true if the font was set to be anti-aliased.
	bool isAntiAliased() const;

	/// \brief Does the font have a full character set?
	/// \returns true if the font was allocated with a full character set.
	bool hasFullCharacterSet() const;
	
	/// \brief Get the number of characters in the loaded character set.
	/// 
	/// If you allocate the font using different parameters, you can load in partial 
	/// and full character sets, this helps you know how many characters it can represent.
	///
	/// \returns Number of characters in loaded character set.
	std::size_t	getNumCharacters() const;

	/// \}
	/// \name Font Size
	/// \{

	/// \brief Returns the size of the font.
	/// \returns Size of font, set when font was loaded.
	int getSize() const;
	
	/// \brief Computes line height based on font size.
	/// \returns the current line height.
	float getLineHeight() const;

	/// \brief Sets line height for text drawn on screen. 
	///
	/// Note the line height is automatically computed based on the font size, when you load in the font.
	///
	/// \param height Line height for text drawn on screen.
	void setLineHeight(float height);

	/// \brief Get the ascender distance for this font.
	///
	/// The ascender is the vertical distance from the baseline to the highest "character" coordinate.
	/// The meaning of "character" coordinate depends on the font. Some fonts take accents into account,
	/// others do not, and still others define it simply to be the highest coordinate over all glyphs.
	///
	/// \returns the font ascender height in pixels.
	float getAscenderHeight() const;

	/// \brief Get the descender distance for this font.
	///
	/// The descender is the vertical distance from the baseline to the lowest "character" coordinate.
	/// The meaning of "character" coordinate depends on the font. Some fonts take accents into account,
	/// others do not, and still others define it simply to be the lowest coordinate over all glyphs.
	/// This value will be negative for descenders below the baseline (which is typical).
	///
	/// \returns the font descender height in pixels.
	float getDescenderHeight() const;

	/// \brief Get the global bounding box for this font.
	///
	/// The global bounding box is the rectangle inside of which all glyphs in the font can fit.
    /// Glyphs are drawn starting from (0,0) in the returned box (though note that the box can
    /// extend in any direction out from the origin).
    ///
	/// \returns the font descender height in pixels.
    const ofRectangle & getGlyphBBox() const;

	/// \brief Returns letter spacing of font object.
	///
	/// You can control this by the ofTrueTypeFont::setLetterSpacing() function. 1.0 = default spacing, 
	/// less then 1.0 would be tighter spacing, greater then 1.0 would be wider spacing.
	///
	/// \returns the letter spacing of font object.
	float getLetterSpacing() const;

	/// \brief Sets the letter spacing of the font object.
	/// 
	/// 1.0 = default spacing, less then 1.0 would be tighter spacing, greater then 1.0 would be wider spacing.
	/// \param spacing Spacing of font object. 
	void setLetterSpacing(float spacing);

	/// \brief Returns a variable that represents how wide spaces are.
	///
	/// It's a scalar for the width of the letter 'p', so 1.0 means that a space will be the size of the lower 
	/// case 'p' of that font. 2.0 means that it's 2 times the size of the lower case 'p', etc.
	///
	/// \returns the width of the space.
	float getSpaceSize() const;

	/// \brief Sets the size of the space ' ' character. 
	/// 
	/// This number, which defaults to 1.0, scales the width of the letter 'p' for the space.
	///
	/// \param size Scales the width of the letter 'p' for the space. 
	void setSpaceSize(float size);

	/// \brief Returns the string width.
	///
	/// This is essentially the width component of the ofTrueTypeFont::getStringBoundingBox() rectangle.
	///
	/// \param s The string to get the width of.
	/// \returns the string width. 
	float stringWidth(const std::string& s) const;

	/// \brief Returns the string height.
	///
	/// This is essentially the height component of the ofTrueTypeFont::getStringBoundingBox() rectangle.
	///
	/// \param s The string to get the height of.
	/// \returns the string height.
	float stringHeight(const std::string& s) const;

	/// \brief Returns the bounding box of a string as a rectangle.
	/// \param s The string to get bounding box of.
	/// \param x X position of returned rectangle.
	/// \param y Y position of returned rectangle.
	/// \returns the bounding box of a string as a rectangle.
	ofRectangle getStringBoundingBox(const std::string& s, float x, float y, bool vflip=true) const;

	/// \}
	/// \name Drawing
	/// \{

	/// \brief Draw a string s at position x,y
	/// \param s String to draw
	/// \param x X position of string
	/// \param y Y position of string
	void drawString(const std::string& s, float x, float y) const;

	/// \brief Draws the string as if it was geometrical shapes.
	/// 
	/// Uses the information contained in ofTTFContour and ofTTFCharacter.
	/// 
	/// \param x X position of shapes
	/// \param y Y position of shapes
	void drawStringAsShapes(const std::string& s, float x, float y) const;
	
	/// \todo
	ofTTFCharacter getCharacterAsPoints(uint32_t character, bool vflip=true, bool filled=true) const;
	vector<ofTTFCharacter> getStringAsPoints(const std::string &  str, bool vflip=true, bool filled=true) const;
	const ofMesh & getStringMesh(const std::string &  s, float x, float y, bool vflip=true) const;
	const ofTexture & getFontTexture() const;
	ofTexture getStringTexture(const std::string &  s, bool vflip=true) const;
	ofVec2f getFirstGlyphPosForTexture(const std::string & str, bool vflip) const;
	bool isValidGlyph(uint32_t) const;
	/// \}

	void setDirection(ofTtfSettings::Direction direction);
protected:
	/// \cond INTERNAL
	
	bool bLoadedOk;
	
	vector <ofTTFCharacter> charOutlines;
	vector <ofTTFCharacter> charOutlinesNonVFlipped;
	vector <ofTTFCharacter> charOutlinesContour;
	vector <ofTTFCharacter> charOutlinesNonVFlippedContour;

	float lineHeight;
	float ascenderHeight;
	float descenderHeight;
	ofRectangle glyphBBox;
	float letterSpacing;
	float spaceSize;
	float fontUnitScale;


	struct glyphProps{
		std::size_t characterIndex;
		uint32_t glyph;
		long height;
		long width;
		long bearingX, bearingY;
		long xmin, xmax, ymin, ymax;
		long advance;
		float tW,tH;
		float t1,t2,v1,v2;
	};

	struct glyph{
		glyphProps props;
		ofPixels pixels;
	};

	vector<glyphProps> cps; // properties for each character

	ofTtfSettings settings;
	unordered_map<uint32_t,size_t> glyphIndexMap;


    int getKerning(uint32_t c, uint32_t prevC) const;
	void drawChar(uint32_t c, float x, float y, bool vFlipped) const;
	void drawCharAsShape(uint32_t c, float x, float y, bool vFlipped, bool filled) const;
	void createStringMesh(const string & s, float x, float y, bool vFlipped) const;
	glyph loadGlyph(uint32_t utf8) const;
	const glyphProps & getGlyphProperties(uint32_t glyph) const;
	void iterateString(const string & str, float x, float y, bool vFlipped, std::function<void(uint32_t, ofVec2f)> f) const;
	size_t indexForGlyph(uint32_t glyph) const;

	ofTexture texAtlas;
	mutable ofMesh stringQuads;

	/// \endcond

private:
#if defined(TARGET_ANDROID) || defined(TARGET_OF_IOS)
	friend void ofUnloadAllFontTextures();
	friend void ofReloadAllFontTextures();
#endif
	shared_ptr<struct FT_FaceRec_>	face;
	static const glyphProps invalidProps;
	void		unloadTextures();
	void		reloadTextures();
	static bool	initLibraries();
	static void finishLibraries();

	friend void ofExitCallback();

};
