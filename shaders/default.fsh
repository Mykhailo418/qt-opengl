struct materialProperty
{
	vec3 diffuseColor;
	vec3 ambienceColor;
	vec3 specularColor;
	float shinnes;
};
uniform sampler2D u_diffuseMap;
uniform sampler2D u_normalMap;
uniform sampler2D u_shadowMap;
uniform highp float u_lightPower;
uniform materialProperty u_materialProperty;
uniform bool u_isUsingDiffuseMap;
uniform bool u_isUsingNormalMap;
varying highp vec4 v_position;
varying highp vec2 v_texCoord;
varying highp vec3 v_normal;
varying highp mat3 v_tbnMatrix;
varying highp vec4 v_lightDirection;
varying highp vec4 v_positionLightMatrix;

float calcShadowFactor(sampler2D map, vec2 coords, float compare) {
	vec4 v = texture2D(map, coords);
	float value = v.x * 255.0 + (v.y * 255.0 + (v.z * 255.0 + v.w) / 255.0) / 255.0;
	return step(compare, value);
}

float getShadowMapLinear(sampler2D map, vec2 coords, float compare, vec2 pixelSize) {
	vec2 pixelPos = coords / pixelSize + 0.5;
	vec2 fractPart = fract(pixelPos);
	vec2 startPixel = (pixelPos - fractPart) * pixelSize;

	float blPixel = calcShadowFactor(map, startPixel, compare);
	float brPixel = calcShadowFactor(map, startPixel + vec2(pixelSize.x, 0.0), compare);
	float tlPixel = calcShadowFactor(map, startPixel + vec2(0.0, pixelSize.y), compare);
	float trPixel = calcShadowFactor(map, startPixel + pixelSize, compare);

	float mixA = mix(blPixel, tlPixel, fractPart.y);
	float mixB = mix(brPixel, trPixel, fractPart.y);

	return mix(mixA, mixB, fractPart.x);
}

float getShadowMapPCF(sampler2D map, vec2 coords, float compare, vec2 pixelSize) {
	float result = 0.0f;
	for(float y = -1.0; y < 1.0; y += 1.0) {
		for(float x = -1.0; x < 1.0; x += 1.0) {
			vec2 offset = vec2(x, y) * pixelSize;
			result += getShadowMapLinear(map, coords + offset, compare, pixelSize);
		}
	}
	return result / 9.0f;
}

float getShadowFactor(sampler2D map, vec4 initialShadowCoords) {
	vec3 tmp = v_positionLightMatrix.xyz / v_positionLightMatrix.w;
	tmp = tmp * vec3(0.5) + vec3(0.5);
	float offset = 2.0 * dot(v_normal, v_lightDirection.xyz);
	return getShadowMapPCF(map, tmp.xy, tmp.z * 255.0 - offset, vec2(1.0f/1024.0f));
}

void main(void)
{
	highp float shadowFactor = getShadowFactor(u_shadowMap, v_positionLightMatrix);

	vec4 resultColor = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 eyePosition = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 diffMatColor = texture2D(u_diffuseMap, v_texCoord);
	vec3 usingNormal = v_normal;
	if(u_isUsingNormalMap) {
		usingNormal = normalize(texture2D(u_normalMap, v_texCoord).rgb * 2.0 - 1.0);
	}
	vec3 eyeVect = normalize(eyePosition.xyz - v_position.xyz);
	if(u_isUsingNormalMap) {
		eyeVect = normalize(v_tbnMatrix * eyeVect);
	}
	vec3 lightVect = normalize(v_lightDirection.xyz);
	if(u_isUsingNormalMap) {
		lightVect = normalize(v_tbnMatrix * lightVect);
	}
	vec3 reflectLight = normalize(reflect(lightVect, usingNormal));
	float len = length(v_position.xyz - eyePosition.xyz);
	float specularFactor = u_materialProperty.shinnes;
	float ambientFactor = 0.1;

	if (u_isUsingDiffuseMap == false) {
		diffMatColor = vec4(u_materialProperty.diffuseColor, 1.0);
	}
	vec4 diffColor = diffMatColor * u_lightPower * max(0.0, dot(usingNormal, -lightVect)); // / (1.0 + 0.25 * len * len);
	resultColor += diffColor;

	vec4 ambientColor = ambientFactor * diffMatColor;
	resultColor += ambientColor * vec4(u_materialProperty.ambienceColor, 1.0);

	/*vec4 specularColor = vec4(1.0, 1.0, 1.0, 1.0) * u_lightPower * pow(max(0.0, dot(reflectLight, -eyeVect)), specularFactor); // / (1.0 + 0.25 * len * len);
	resultColor += specularColor * vec4(u_materialProperty.specularColor, 1.0);*/

	shadowFactor += 0.2;
	shadowFactor = shadowFactor > 1.0 ? 1.0 : shadowFactor;

	gl_FragColor = resultColor * shadowFactor;
	//gl_FragColor = diffMatColor;
};