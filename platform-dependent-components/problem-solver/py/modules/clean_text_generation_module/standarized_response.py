from dataclasses import dataclass
from typing import Optional

@dataclass
class StandardizedResponse:
    text: str
    is_error: bool = False
    error_message: Optional[str] = None

def get_standarized_response(response_url: str, raw_response: dict, user_provided_parser: Optional[callable] = None) -> StandardizedResponse:
    if user_provided_parser:
        try:
            return user_provided_parser(raw_response)
        except Exception as e:
            return StandardizedResponse(
                text="",
                is_error=True,
                error_message=f"Custom parser failed: {str(e)}"
            )

    return get_adapter(response_url)(raw_response)

def get_adapter(api_url: str) -> callable:
    if "api.openai.com" in api_url:
        return adapt_openai_response
    elif "api.groq.com" in api_url:
        return adapt_groq_response
    else:
        return adapt_generic_response

def adapt_openai_response(raw_response: dict) -> StandardizedResponse:
    try:
        return StandardizedResponse(
            text=raw_response["choices"][0]["message"]["content"]
        )
    except KeyError as outer_e:
        try:
            return StandardizedResponse(
                text=raw_response["output"][0]["content"][0]["text"]
            )
        except KeyError as e:
            return StandardizedResponse(
                text="",
                is_error=True,
                error_message=f"Missing key in OpenAI response: {str(e) and str(outer_e)}"
            )

def adapt_groq_response(raw_response: dict) -> StandardizedResponse:
    try:
        return StandardizedResponse(
            text=raw_response["choices"][0]["message"]["content"]
        )
    except KeyError as e:
        return StandardizedResponse(
            text="",
            is_error=True,
            error_message=f"Missing key in Groq response: {str(e)}"
        )
def adapt_generic_response(raw_response: dict) -> StandardizedResponse:
    possible_text_fields = [
        "text", "output", "response", "content",
        "message", "generated_text", "completion"
    ]

    for field in possible_text_fields:
        if field in raw_response:
            return StandardizedResponse(text=str(raw_response[field]))

    from collections.abc import Mapping
    def find_text(data):
        if isinstance(data, str):
            return data
        if isinstance(data, Mapping):
            for v in data.values():
                result = find_text(v)
                if result:
                    return result
        elif isinstance(data, (list, tuple)):
            for item in data:
                result = find_text(item)
                if result:
                    return result
        return None

    found_text = find_text(raw_response)
    if found_text:
        return StandardizedResponse(text=found_text)

    return StandardizedResponse(
        text="",
        is_error=True,
        error_message="Could not extract text from API response"
    )
