#pragma once
#include <vector>
#include "CarRenderInfo.h"
#include "ePoly.h"
#include "eView.h"

inline float screenWidth = 1920;
inline float screenHeight = 1080;

inline void sub_5BC4A0(eView* view, bVector3* out, bVector3* v)
{
	double v3 = v->z * view->PlatInfo->ProjectionZBiasMatrix.v2.y
		+ v->x * view->PlatInfo->ProjectionZBiasMatrix.v0.y
		+ v->y * view->PlatInfo->ProjectionZBiasMatrix.v1.y
		+ view->PlatInfo->ProjectionZBiasMatrix.v3.y;
	float v9 = v->z * view->PlatInfo->ProjectionZBiasMatrix.v2.z
		+ v->x * view->PlatInfo->ProjectionZBiasMatrix.v0.z
		+ v->y * view->PlatInfo->ProjectionZBiasMatrix.v1.z
		+ view->PlatInfo->ProjectionZBiasMatrix.v3.z;
	float v10 = v->z * view->PlatInfo->ProjectionZBiasMatrix.v2.w
		+ v->x * view->PlatInfo->ProjectionZBiasMatrix.v0.w
		+ v->y * view->PlatInfo->ProjectionZBiasMatrix.v1.w
		+ view->PlatInfo->ProjectionZBiasMatrix.v3.w;
	float v5 = v->z * view->PlatInfo->ProjectionZBiasMatrix.v2.x
		+ v->y * view->PlatInfo->ProjectionZBiasMatrix.v1.x
		+ v->x * view->PlatInfo->ProjectionZBiasMatrix.v0.x
		+ view->PlatInfo->ProjectionZBiasMatrix.v3.x;

	float v4 = 1.0 / v10;
	float v6 = v4 * v5;

	out->x = (v6 + 1.0) * (float)*Game::ScreenSizeX * 0.5;
	out->y = (v3 * v4 - 1.0) * (float)*Game::ScreenSizeY * (-0.5);
	out->z = v4 * v9;
}

struct Neon
{
	bMatrix4* Start;
	bMatrix4* End;
};

class CarNeon
{
private:
	CarRenderInfo* carRenderInfo;
	bMatrix4* carMatrix;
	std::vector<Neon> neons;
	bMatrix4 mIdentity;

public:
	CarNeon(CarRenderInfo* carRenderInfo, bMatrix4* carMatrix)
	{
		this->carRenderInfo = carRenderInfo;
		this->carMatrix = carMatrix;
		this->mIdentity.Identity();
	}

	void Update()
	{

	}

	void FindMarkers()
	{
		auto rideInfo = this->carRenderInfo->RideInfo;
		Slot slots[] = { Slot::FRONT_BUMPER, Slot::REAR_BUMPER };
		for (Slot slot : slots)
		{
			auto part = rideInfo->GetPart(slot);
			if (part)
			{
				for (int i = 0; i < 99; i++)
				{
					char buff[64];
					sprintf_s(buff, "NEON_START%d", i);
					auto start = part->GetMarker(StringHash(buff));
					if (start)
					{
						sprintf_s(buff, "NEON_END%d", i);
						auto end = part->GetMarker(StringHash(buff));
						neons.push_back({ (bMatrix4*)start, (bMatrix4*)end });
					}
					else
					{
						break;
					}
				}
			}
		}
	}

	void RenderMarkers()
	{
		for (auto neon : neons)
		{
			RenderMarker(neon.Start, neon.End);
		}
	}

	void RenderMarker(bMatrix4* startMatrix, bMatrix4* endMatrix)
	{
		ePoly poly;
		auto textureInfo = GetTextureInfo(StringHash("NEONBLUR"), 0, 0);
		auto carPos = this->carMatrix->v3;
		auto carMatrix = *this->carMatrix;
		carMatrix.v3 = { 0,0,0,0 };

		auto view = eView::GetPlayer1();
		auto camera = view->pCamera;

		bVector2 v35;
		bVector2 v83;
		bVector3 a2;
		bVector3 a3;
		bVector3 v43;
		bVector3 v239;
		float a8 = 0.03;

		float v12 = carPos.x;
		float v14 = v12 - camera->CurrentKey.Position.x;
		float v15 = camera->CurrentKey.Matrix.v1.x;
		float v16 = carPos.y - camera->CurrentKey.Position.y;
		float v17 = camera->CurrentKey.Position.z;
		a3.x = camera->CurrentKey.Matrix.v0.x;
		float v18 = carPos.z - v17;
		float v19 = camera->CurrentKey.Matrix.v2.x;
		a3.y = v15;
		float v20 = camera->CurrentKey.Matrix.v0.y;
		a3.z = v19;
		float v21 = camera->CurrentKey.Matrix.v1.y;
		v43.x = v20;
		float v22 = camera->CurrentKey.Matrix.v2.y;
		v43.y = v21;
		v43.z = v22;
		float v45 = 1.0;
		float v39 = sqrt(v18 * v18 + v14 * v14 + v16 * v16);

		float v36 = v45 * a8;

		float v50 = v36 * v43.x;
		v45 = v36 * v43.y;
		float v46 = v36 * v43.z;
		a2.z = v46;
		float v146 = v46;
		float v144 = v50;
		float v145 = v45;
		float v40 = v36 * a3.x;
		float v41 = v36 * a3.y;
		float v37 = v36 * a3.z;
		float v42 = -v43.x;
		float v38 = -v43.y;
		v39 = -v43.z;

		a3.x = 0.0;
		a3.y = 0.0;
		a3.z = 0.0;
		eMulVector(&a3, startMatrix, &a3);
		eMulVector(&a3, &carMatrix, &a3);
		a3.x = a3.x + carPos.x;
		a3.y = a3.y + carPos.y;
		v43.x = 0.0;
		v43.y = 0.0;
		float v28 = a3.z + carPos.z;
		v43.z = 0.0;
		a3.z = v28;
		eMulVector(&v43, endMatrix, &v43);
		eMulVector(&v43, &carMatrix, &v43);
		v43.x = v43.x + carPos.x;
		v43.y = v43.y + carPos.y;
		v43.z = v43.z + carPos.z;
		float v162 = v43.x - a3.x;
		float v163 = v43.y - a3.y;
		float v164 = v43.z - a3.z;
		sub_5BC4A0(view, &a2, &a3);
		sub_5BC4A0(view, &v239, &v43);
		v35.x = v239.x - a2.x;
		v35.y = v239.y - a2.y;
		bNormalize(&v83, &v35);
		v35 = v83;
		float v141 = v40;
		poly.Vertices[0].y = a3.y;
		float v142 = v41;
		poly.Vertices[0].x = a3.x;
		poly.Vertices[0].z = a3.z;
		poly.Vertices[1].y = v43.y;
		poly.Vertices[1].x = v43.x;
		poly.Vertices[1].z = v43.z;
		float v143 = v37;
		float v282 = v37 * v35.y;
		float v244 = v37;
		float v207 = v40 * v35.y;
		float v208 = v282;
		float v237 = v38;
		poly.Vertices[2].x = v43.x - v207;
		float v238 = v39;
		float v248 = v37;
		float v246 = v39 * v36;
		float v67 = v246;
		float v65 = v42 * v36;
		float v119 = v37;
		float v66 = v38 * v36;
		float v236 = v39;
		float v310 = v246 * v35.x;
		float v174 = v310;
		float v173 = v65 * v35.x;
		float v235 = v38;
		poly.Vertices[2].x = poly.Vertices[2].x - v173;
		poly.Vertices[2].y = v43.y - v41 * v35.y - v66 * v35.x;
		poly.Vertices[2].z = v43.z - v282 - v310;
		float v117 = v40;
		float v118 = v41;
		float v284 = v282;
		float v250 = v207;
		poly.Vertices[3].x = a3.x - v207;
		poly.Vertices[3].y = a3.y - v41 * v35.y;
		float v302 = v246;
		float v73 = v246;
		float v71 = v65;
		float v72 = v66;
		float v252 = v310;
		float v232 = v310;
		float v231 = v173;
		float v30 = 0;
		poly.Vertices[3].x = poly.Vertices[3].x - v173;
		poly.Vertices[3].y = poly.Vertices[3].y - v66 * v35.x;
		poly.Vertices[3].z = a3.z - v282 - v310;

		poly.UVs[0].x = 0.5;
		poly.UVs[0].y = 0.5;
		poly.UVs[1].x = 0.5;
		poly.UVs[1].y = 0.5;
		poly.UVs[2].x = 0.5;
		poly.UVs[2].y = 0.0;
		poly.UVs[3].x = 0.5;
		poly.UVs[3].y = 0.0;
		poly.Render(textureInfo, &this->mIdentity);

		float v165 = v40;
		float v167 = v37;
		float v166 = v41;
		float v286 = v37;
		float v177 = v38;
		float v178 = v39;
		float v254 = v37 * v35.y;
		float v79 = v254;
		float v77 = v40 * v35.y;
		float v78 = v41 * v35.y;
		float v318 = v254 + v254;
		float v256 = v77 + v77;
		poly.Vertices[2].x = v256 + poly.Vertices[2].x;
		poly.Vertices[2].y = v78 + v78 + poly.Vertices[2].y;
		float v288 = v39 * v36;
		float v125 = v288;
		float v123 = v42 * v36;
		float v124 = v38 * v36;
		float v258 = v288 * v35.x;
		float v58 = v258;
		float v56 = v123 * v35.x;
		float v57 = v124 * v35.x;
		float v149 = v37;
		float v304 = v258 + v258;
		float v210 = v304;
		float v209 = v56 + v56;
		float v260 = v37;
		float v181 = v38;
		float v182 = v39;
		poly.Vertices[2].x = v209 + poly.Vertices[2].x;
		poly.Vertices[2].y = v57 + v57 + poly.Vertices[2].y;
		poly.Vertices[2].z = v304 + v318 + poly.Vertices[2].z;
		float v147 = v40;
		float v148 = v41;
		float v290 = v254;
		float v89 = v254;
		float v87 = v77;
		float v88 = v78;
		float v262 = v290 + v290;
		float v240 = v87 + v87;
		poly.Vertices[3].x = v240 + poly.Vertices[3].x;
		poly.Vertices[3].y = v88 + v88 + poly.Vertices[3].y;
		float v264 = v288;
		float v131 = v288;
		float v129 = v123;
		float v130 = v124;
		float v292 = v258;
		float v95 = v258;
		float v93 = v56;
		float v94 = v57;
		float v266 = v292 + v292;
		float v226 = v266;
		float v225 = v93 + v93;
		poly.Vertices[3].x = v225 + poly.Vertices[3].x;
		poly.Vertices[3].y = v94 + v94 + poly.Vertices[3].y;
		poly.Vertices[3].z = v266 + v262 + poly.Vertices[3].z;
		poly.Render(textureInfo, &this->mIdentity);

		float v159 = v40;
		float v160 = v41;
		float v161 = v37;
		float v268 = v37 * v35.y;
		float v185 = v40 * v35.y;
		float v186 = v268;
		float v306 = v37;
		float v213 = v38;
		poly.Vertices[0].x = a3.x - v185;
		float v214 = v39;
		float v294 = v39 * v36;
		float v99 = v42 * v36;
		float v101 = v294;
		float v100 = v38 * v36;
		float v137 = v37;
		float v316 = v37;
		float v234 = v39;
		float v270 = v294 * v35.x;
		float v190 = v270;
		float v189 = v99 * v35.x;
		float v233 = v38;
		poly.Vertices[0].x = poly.Vertices[0].x - v189;
		poly.Vertices[0].y = a3.y - v41 * v35.y - v100 * v35.x;
		poly.Vertices[0].z = a3.z - v268 - v270;
		float v135 = v40;
		float v136 = v41;
		float v272 = v268;
		float v296 = v185;
		poly.Vertices[1].x = a3.x - v185;
		poly.Vertices[1].y = a3.y - v41 * v35.y;
		float v274 = v294;
		float v107 = v294;
		float v105 = v99;
		float v106 = v100;
		float v308 = v270;
		float v194 = v270;
		float v193 = v189;
		float v155 = v37;
		float v276 = v37;
		poly.Vertices[1].x = poly.Vertices[1].x - v189;
		float v113 = v46;
		float v278 = v46;
		poly.Vertices[1].y = poly.Vertices[1].y - v100 * v35.x;
		float v61 = v37;
		float v280 = v37;
		float v153 = v40;
		float v154 = v41;
		float v298 = v37 * v35.x;
		float v218 = v298;
		float v217 = v40 * v35.x;
		poly.Vertices[1].x = poly.Vertices[1].x - v217;
		poly.Vertices[1].y = poly.Vertices[1].y - v41 * v35.x;
		float v111 = v50;
		float v112 = v45;
		float v314 = v46 * v35.y;
		float v198 = v314;
		float v197 = v50 * v35.y;
		poly.Vertices[1].x = poly.Vertices[1].x - v197;
		poly.Vertices[1].y = poly.Vertices[1].y - v45 * v35.y;
		poly.Vertices[1].z = a3.z - v268 - v270 - v298 - v314;
		float v59 = v40;
		float v60 = v41;
		float v241 = v268;
		float v243 = v185;
		float v229 = v38;
		float v230 = v39;
		float v249 = v37;
		poly.Vertices[2].x = v185 + a3.x;
		poly.Vertices[2].y = v41 * v35.y + a3.y;
		float v245 = v294;
		float v64 = v294;
		float v62 = v99;
		float v70 = v37;
		float v63 = v100;
		float v253 = v46;
		float v76 = v46;
		float v247 = v270;
		float v202 = v270;
		float v201 = v35.x * v99;
		poly.Vertices[2].x = poly.Vertices[2].x + v201;
		poly.Vertices[2].y = v100 * v35.x + poly.Vertices[2].y;
		float v68 = v40;
		float v69 = v41;
		float v251 = v298;
		float v222 = v298;
		float v221 = v35.x * v40;
		poly.Vertices[2].x = poly.Vertices[2].x - v221;
		poly.Vertices[2].y = poly.Vertices[2].y - v41 * v35.x;
		float v74 = v50;
		float v75 = v45;
		float v82 = v37;
		float v257 = v37;
		float v172 = v39;
		float v255 = v314;
		float v206 = v314;
		float v205 = v35.y * v50;
		float v171 = v38;
		poly.Vertices[2].x = poly.Vertices[2].x - v205;
		poly.Vertices[2].y = poly.Vertices[2].y - v45 * v35.y;
		poly.Vertices[2].z = v268 + a3.z + v270 - v298 - v314;
		float v80 = v40;
		float v81 = v41;
		float v259 = v268;
		float v261 = v35.y * v40;
		poly.Vertices[3].x = a3.x + v261;
		poly.Vertices[3].y = v41 * v35.y + a3.y;
		float v263 = v294;
		float v86 = v294;
		float v84 = v99;
		float v85 = v100;
		float v265 = v270;
		float v176 = v270;
		float v175 = v201;
		poly.Vertices[3].x = poly.Vertices[3].x + v201;
		poly.Vertices[3].y = v100 * v35.x + poly.Vertices[3].y;
		poly.Vertices[3].z = v270 + v268 + a3.z;
		poly.UVs[0].x = 0.5;
		poly.UVs[0].y = 0.0;
		poly.UVs[1].x = 0.0;
		poly.UVs[1].y = 0.0;
		poly.UVs[2].x = 0.0;
		poly.UVs[2].y = 1.0;
		poly.UVs[3].x = 0.5;
		poly.UVs[3].y = 1.0;
		poly.Render(textureInfo, &this->mIdentity);

		float v90 = v40;
		float v91 = v41;
		float v92 = v37;
		float v269 = v37 * v35.y;
		float v179 = v40 * v35.y;
		float v180 = v269;
		float v267 = v37;
		float v183 = v38;
		poly.Vertices[0].x = v43.x - v179;
		float v184 = v39;
		float v271 = v39 * v36;
		float v98 = v271;
		float v96 = v42 * v36;
		float v97 = v38 * v36;
		float v273 = v271 * v35.x;
		float v187 = v96 * v35.x;
		float v188 = v273;
		float v104 = v37;
		poly.Vertices[0].x = poly.Vertices[0].x - v187;
		float v275 = v37;
		float v191 = v38;
		float v192 = v39;
		poly.Vertices[0].y = v43.y - v41 * v35.y - v97 * v35.x;
		float v285 = v37;
		poly.Vertices[0].z = v43.z - v269 - v273;
		float v102 = v40;
		float v103 = v41;
		float v277 = v269;
		float v279 = v179;
		poly.Vertices[1].x = v43.x - v179;
		poly.Vertices[1].y = v43.y - v41 * v35.y;
		float v281 = v271;
		float v110 = v271;
		float v108 = v96;
		float v109 = v97;
		float v283 = v273;
		float v196 = v273;
		float v195 = v187;
		poly.Vertices[1].x = poly.Vertices[1].x - v187;
		poly.Vertices[1].y = poly.Vertices[1].y - v97 * v35.x;
		float v114 = v40;
		float v116 = v37;
		float v115 = v41;
		float v122 = v46;
		float v289 = v46;
		float v128 = v37;
		float v293 = v37;
		float v287 = v37 * v35.x;
		float v200 = v287;
		float v199 = v40 * v35.x;
		poly.Vertices[1].x = poly.Vertices[1].x + v199;
		poly.Vertices[1].y = v41 * v35.x + poly.Vertices[1].y;
		float v120 = v50;
		float v121 = v45;
		float v291 = v46 * v35.y;
		float v204 = v291;
		float v203 = v50 * v35.y;
		float v169 = v38;
		poly.Vertices[1].x = poly.Vertices[1].x + v203;
		poly.Vertices[1].y = v45 * v35.y + poly.Vertices[1].y;
		poly.Vertices[1].z = v291 + v43.z - v269 - v273 + v287;
		float v126 = v40;
		float v127 = v41;
		float v295 = v269;
		float v297 = v179;
		poly.Vertices[2].x = v43.x + v179;
		poly.Vertices[2].y = v41 * v35.y + v43.y;
		float v170 = v39;
		float v303 = v37;
		float v299 = v271;
		float v134 = v271;
		float v132 = v96;
		float v140 = v37;
		float v133 = v97;
		float v309 = v37;
		float v301 = v273;
		float v212 = v273;
		float v211 = v187;
		poly.Vertices[2].x = poly.Vertices[2].x + v187;
		poly.Vertices[2].y = v97 * v35.x + poly.Vertices[2].y;
		float v138 = v40;
		float v139 = v41;
		float v305 = v287;
		float v216 = v287;
		float v215 = v199;
		poly.Vertices[2].x = poly.Vertices[2].x + v199;
		poly.Vertices[2].y = v41 * v35.x + poly.Vertices[2].y;
		float v307 = v146 * v35.y;
		float v220 = v307;
		float v219 = v144 * v35.y;
		poly.Vertices[2].x = poly.Vertices[2].x + v219;
		poly.Vertices[2].y = v145 * v35.y + poly.Vertices[2].y;
		poly.Vertices[2].z = v307 + v269 + v43.z + v273 + v287;
		float v150 = v40;
		float v151 = v41;
		float v152 = v37;
		float v223 = v38;
		float v224 = v39;
		float v311 = v269;
		float v313 = v179;
		poly.UVs[0].x = 0.5;
		poly.UVs[0].y = 0.0;
		poly.UVs[1].x = 1.0;
		poly.UVs[1].y = 0.0;
		poly.UVs[2].x = 1.0;
		poly.UVs[2].y = 1.0;
		poly.Vertices[3].x = v43.x + v179;
		poly.UVs[3].x = 0.5;
		poly.UVs[3].y = 1.0;
		poly.Vertices[3].y = v41 * v35.y + v43.y;
		float v315 = v271;
		float v158 = v271;
		float v156 = v96;
		float v157 = v97;
		float v317 = v273;
		float v228 = v273;
		float v227 = v187;
		poly.Vertices[3].x = poly.Vertices[3].x + v187;
		poly.Vertices[3].y = v97 * v35.x + poly.Vertices[3].y;
		poly.Vertices[3].z = v273 + v269 + v43.z;
		poly.Render(textureInfo, &this->mIdentity);
	}
};